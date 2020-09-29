// L5-eval-box

import { map, repeat, zipWith } from "ramda";
import { CExp, Exp, IfExp, LetrecExp, LetExp, ProcExp, Program, SetExp, isCExp, LetValue, isValuesExp, isLetValuesExp, isBindings_Values, Values, Bindings_Values, parseL5CExp } from './L5-ast';
import { Binding, VarDecl } from "./L5-ast";
import { isBoolExp, isLitExp, isNumExp, isPrimOp, isStrExp, isVarRef } from "./L5-ast";
import { parseL5Exp } from "./L5-ast";
import { isAppExp, isDefineExp, isIfExp, isLetrecExp, isLetExp,
         isProcExp, isSetExp } from "./L5-ast";
import { applyEnv, applyEnvBdg, globalEnvAddBinding, makeExtEnv, setFBinding,
         theGlobalEnv, Env, FBinding, ExtEnv } from "./L5-env";
import { isClosure, makeClosure, Closure, Value, SExpValue, makeTuple, Tuple, isTuple } from "./L5-value";
import { isEmpty, first, rest, cons } from '../shared/list';
import { Result, makeOk, makeFailure, mapResult, safe2, bind, isOk } from "../shared/result";
import { parse as p } from "../shared/parser";
import { applyPrimitive } from "./evalPrimitive";

// ========================================================
// Eval functions

export const applicativeEval = (exp: CExp, env: Env): Result<Value> =>
    isNumExp(exp) ? makeOk(exp.val) :
    isBoolExp(exp) ? makeOk(exp.val) :
    isStrExp(exp) ? makeOk(exp.val) :
    isPrimOp(exp) ? makeOk(exp) :
    isVarRef(exp) ? applyEnv(env, exp.var) :
    isLitExp(exp) ? makeOk(exp.val) :
    isIfExp(exp) ? evalIf(exp, env) :
    isProcExp(exp) ? evalProc(exp, env) :
    isLetExp(exp) ? evalLet(exp, env) :
    isLetrecExp(exp) ? evalLetrec(exp, env) :
    isSetExp(exp) ? evalSet(exp, env) :
    isAppExp(exp) ? safe2((proc: Value, args: Value[]) => applyProcedure(proc, args))
                        (applicativeEval(exp.rator, env), mapResult(rand => applicativeEval(rand, env), exp.rands)) :
    isValuesExp(exp)? evalValue(exp,env) :
    isLetValuesExp(exp) ? evalLetValue(exp, env):

    makeFailure(`Bad L5 AST ${exp}`);

export const isTrueValue = (x: Value): boolean =>
    ! (x === false);

const evalIf = (exp: IfExp, env: Env): Result<Value> =>
    bind(applicativeEval(exp.test, env),
         (test: Value) => isTrueValue(test) ? applicativeEval(exp.then, env) : applicativeEval(exp.alt, env));

const evalProc = (exp: ProcExp, env: Env): Result<Closure> =>
    makeOk(makeClosure(exp.args, exp.body, env));

// KEY: This procedure does NOT have an env parameter.
//      Instead we use the env of the closure.
const applyProcedure = (proc: Value, args: Value[]): Result<Value> =>
    isPrimOp(proc) ? applyPrimitive(proc, args) :
    isClosure(proc) ? applyClosure(proc, args) :
    makeFailure(`Bad procedure ${JSON.stringify(proc)}`);

const applyClosure = (proc: Closure, args: Value[]): Result<Value> => {
    const vars = map((v: VarDecl) => v.var, proc.params);
    return evalSequence(proc.body, makeExtEnv(vars, args, proc.env));
}

const evalValue = (value:Values, env:Env): Result<Value> => {
   let vars: Result<SExpValue[]>= mapResult((v: CExp) => applicativeEval(v, env), value.values);
   if(isOk(vars)) 
         return makeOk(makeTuple(vars.value))
   else
         return makeFailure("not good values")
}

// Evaluate a sequence of expressions (in a program)
export const evalSequence = (seq: Exp[], env: Env): Result<Value> =>
    isEmpty(seq) ? makeFailure("Empty sequence") :
    isDefineExp(first(seq)) ? evalDefineExps(first(seq), rest(seq)) :
    evalCExps(first(seq), rest(seq), env);
    
const evalCExps = (first: Exp, rest: Exp[], env: Env): Result<Value> =>
    isCExp(first) && isEmpty(rest) ? applicativeEval(first, env) :
    isCExp(first) ? bind(applicativeEval(first, env), _ => evalSequence(rest, env)) :
    makeFailure("Never");
    
// define always updates theGlobalEnv
// We also only expect defineExps at the top level.
// Eval a sequence of expressions when the first exp is a Define.
// Compute the rhs of the define, extend the env with the new binding
// then compute the rest of the exps in the new env.
const evalDefineExps = (def: Exp, exps: Exp[]): Result<Value> =>
    isDefineExp(def) ? bind(applicativeEval(def.val, theGlobalEnv),
                            (rhs: Value) => { globalEnvAddBinding(def.var.var, rhs);
                                              return evalSequence(exps, theGlobalEnv); }) :
    makeFailure("Unexpected " + def);

// Main program
export const evalProgram = (program: Program): Result<Value> =>
    evalSequence(program.exps, theGlobalEnv);

export const evalParse = (s: string): Result<Value> =>
    bind(bind(p(s), parseL5Exp), (exp: Exp) => evalSequence([exp], theGlobalEnv));

// LET: Direct evaluation rule without syntax expansion
// compute the values, extend the env, eval the body.
const evalLet = (exp: LetExp, env: Env): Result<Value> => {
    const vals = mapResult((v : CExp) => applicativeEval(v, env), map((b : Binding) => b.val, exp.bindings));
    const vars = map((b: Binding) => b.var.var, exp.bindings);
    return bind(vals, (vals: Value[]) => evalSequence(exp.body, makeExtEnv(vars, vals, env)));
}

const evalLetValue = (exp: LetValue, env: Env): Result<Value> => {
    const vals = mapResult((v : CExp) => applicativeEval(v, env), map((b : Bindings_Values) => b.exp, exp.bindings));
    const vals1 = evalSexp(vals) 
    const vars = map((b: Bindings_Values) => b.bindings ,exp.bindings);
    var merged = vars.reduce(function(prev, next) {
        return prev.concat(next);
    });
    let valid: Boolean= isOk(vals) ? check_ok(vars, vals.value) : false;
    if(!valid) {return makeFailure("not valid exps")} 
    const vars1 = map((b: VarDecl) => b.var, merged);
    return bind(vals1, (vals: Value[]) => evalSequence(exp.exp, makeExtEnv(vars1, vals, env)))
}

const check_ok = (exp1: VarDecl[][], vals:SExpValue[]): Boolean =>{
    let answer: Boolean= true;
    for(let i=0 ;i<vals.length; i++){
        let val2: SExpValue= vals[i]
        isTuple(val2) && val2.val.length!=exp1[i].length ? answer=false:
        !isTuple(val2) ? answer=false : ""
    }
    return answer;
}

const evalSexp = (exp: Result<SExpValue[]>): Result<Value[]> => {
    if(isOk(exp)){
    let result: Value[]=[]
    var merged = exp.value.map((x) => isTuple(x) ? result.concat(x.val): [])
    var return_value:Value[] = merged.reduce(function(prev, next) {
        return prev.concat(next);
    });
    return makeOk(return_value);
    }
    return makeFailure("not an array")
}

// LETREC: Direct evaluation rule without syntax expansion
// 1. extend the env with vars initialized to void (temporary value)
// 2. compute the vals in the new extended env
// 3. update the bindings of the vars to the computed vals
// 4. compute body in extended env
const evalLetrec = (exp: LetrecExp, env: Env): Result<Value> => {
    const vars = map((b) => b.var.var, exp.bindings);
    const vals = map((b) => b.val, exp.bindings);
    const extEnv = makeExtEnv(vars, repeat(undefined, vars.length), env);
    // @@ Compute the vals in the extended env
    const cvalsResult = mapResult((v: CExp) => applicativeEval(v, extEnv), vals);
    const result = bind(cvalsResult,
                        (cvals: Value[]) => makeOk(zipWith((bdg, cval) => setFBinding(bdg, cval), extEnv.frame.fbindings, cvals)));
    return bind(result, _ => evalSequence(exp.body, extEnv));
};

// const evalLetValue = (exp:LetValue, env:Env): Result<Value> =>{
//     const var1= exp.bindings.var.var;
//     const val= exp.bindings.val;
//     const extEnv = makeExtEnv([var1], repeat(undefined, 1), env);
//     const cvalResult = applicativeEval(val, extEnv);

// }

// L4-eval-box: Handling of mutation with set!
const evalSet = (exp: SetExp, env: Env): Result<void> =>
    safe2((val: Value, bdg: FBinding) => makeOk(setFBinding(bdg, val)))
        (applicativeEval(exp.val, env), applyEnvBdg(env, exp.var.var));
