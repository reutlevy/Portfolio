// ========================================================
// L4 normal eval
import { Sexp } from "s-expression";
//import { map } from "ramda";
import { CExp, Exp, IfExp, Program, parseL4Exp, isLetExp, isLetrecExp, ProcExp, VarDecl, LetExp, Binding, LetrecExp, NumExp, BoolExp, StrExp, LitExp, PrimOp, makeNumExp, makeBoolExp, makeStrExp, makeProcExp, makeLitExp } from "./L4-ast";
import { isAppExp, isBoolExp, isCExp, isDefineExp, isIfExp, isLitExp, isNumExp,
         isPrimOp, isProcExp, isStrExp, isVarRef } from "./L4-ast";
import { applyEnv, makeEmptyEnv, Env, makeExtEnv, makeRecEnv, applyExtEnv, applyRecEnv } from './L4-env-normal';
//import { isTrueValue } from "./L4-eval";
import { applyPrimitive } from "./evalPrimitive";
import { Value, Closure, isClosure, makeClosure } from "./L4-value";
import { first, rest, isEmpty, allT } from '../shared/list';
import { Result, makeOk, makeFailure, mapResult, safe2, bind } from "../shared/result";
import { parse as p } from "../shared/parser";
import { map } from "ramda";
import { isNumber, isBoolean, isString } from "util";
import { AppExp, VarRef } from "../L3/L3-ast";

// Evaluate a sequence of expressions (in a program)
export const evalExps = (exps: Exp[], env: Env): Result<Value> =>
isEmpty(exps) ? makeFailure("Empty program") :
isDefineExp(first(exps)) ? evalDefineExp(first(exps), rest(exps), env) :
evalCExpsL4(first(exps), rest(exps), env);

const evalCExpsL4 = (exp1: Exp, exps: Exp[], env: Env): Result<Value> => 
    (isCExp(exp1) && isEmpty(exps)) ? normalEval(exp1, env) :
    isCExp(exp1) ? bind(normalEval(exp1, env), _ => evalExps(exps, env)) :
     makeFailure("Never");


const evalDefineExp = (def: Exp, exps: Exp[], env: Env): Result<Value> =>
    isDefineExp(def) ? evalExps(exps, makeExtEnv([def.var.var], [def.val], env)) :
    makeFailure("error")
    
// Eval a sequence of expressions when the first exp is a Define.
// Compute the rhs of the define, extend the env with the new binding
// then compute the rest of the exps in the new env.

export const evalNormalProgram = (program: Program): Result<Value> =>
     evalExps(program.exps, makeEmptyEnv());

export const evalNormalParse = (s: string): Result<Value> =>
    bind(p(s),
         (parsed: Sexp) => bind(parseL4Exp(parsed),
                                (exp: Exp) => evalExps([exp], makeEmptyEnv())));


// ============================================================
// L4-eval to L4-normal

const normalEval = (exp: CExp, env: Env): Result<Value> =>
    isNumExp(exp) ? makeOk(exp.val) :
    isBoolExp(exp) ? makeOk(exp.val) :
    isStrExp(exp) ? makeOk(exp.val) :
    isPrimOp(exp) ? makeOk(exp) :
    isVarRef(exp) ? applyVarRef(exp, env) :
    isLitExp(exp) ? makeOk(exp.val) :
    isIfExp(exp) ? evalIf(exp, env) :
    isProcExp(exp) ? evalProc(exp, env) :
    isLetExp(exp) ? evalLet(exp, env) :
    isAppExp(exp) ? bind(normalEval(exp.rator,env), (y:Value) => applyProcedure(y, exp.rands, env)):
    makeFailure(`Bad L4 AST ${exp}`);

export const isTrueValue = (x: Value): boolean =>
    ! (x === false);

const evalIf = (exp: IfExp, env: Env): Result<Value> =>
    bind(normalEval(exp.test, env),
         (test: Value) => isTrueValue(test) ? normalEval(exp.then, env) : normalEval(exp.alt, env));  

const evalProc = (exp: ProcExp, env: Env): Result<Closure> =>
    makeOk(makeClosure(exp.args, exp.body, env));

export const applyVarRef = (exp:VarRef, env:Env) : Result<Value> =>
     bind(applyEnv(env,exp.var), (exp:CExp) => normalEval(exp, env))


export const applyProcedure = (proc: Value, procs: CExp[], env: Env): Result<Value> => 
    isPrimOp(proc) ? bind( mapResult((vars) => normalEval(vars, env), procs), (procs: Value[]) => applyPrimitive(proc, procs)):
    isClosure(proc) ? evalExps(proc.body, makeExtEnv(map((v) => v.var,proc.params), procs, env)) :
    makeFailure(`Bad proc applied ${proc}`);

const applyClosure = (proc: Closure, args: Value[]): Result<Value> => {
    const vars = map((v: VarDecl) => v.var, proc.params);
    return evalExps(proc.body, makeExtEnv(vars, args.map((x)=>valueToLitExp(x)), proc.env));
}

// LET: Direct evaluation rule without syntax expansion
// compute the values, extend the env, eval the body.
const evalLet = (exp: LetExp, env: Env): Result<Value> => {
    const vals = mapResult((v: CExp) => normalEval(v, env), map((b: Binding) => b.val, exp.bindings));
    const vars = map((b: Binding) => b.var.var, exp.bindings);
    return bind(vals, (vals: Value[]) => evalExps(exp.body, makeExtEnv(vars, vals.map((x)=>valueToLitExp(x)), env)));
}

export const valueToLitExp = (v: Value): NumExp | BoolExp | StrExp | LitExp | PrimOp | ProcExp =>
    isNumber(v) ? makeNumExp(v) :
    isBoolean(v) ? makeBoolExp(v) :
    isString(v) ? makeStrExp(v) :
    isPrimOp(v) ? v :
    isClosure(v) ? makeProcExp(v.params, v.body) :
    makeLitExp(v);



