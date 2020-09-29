import {Graph, makeGraph, makeHeaderExp, GraphContent, makeCompoundGraph, makeEdgeExp, makeNodeRef, Edge, makeNodeDecl, isCompoundGraph, makeAtomicGraph, isAtomicGraph, CompoundGraph, parseCompound, NodeDecl} from './mermaid-ast'
import {Parsed, isProgram, isDefineExp, isProcExp, ProcExp, CExp, IfExp, makeVarRef, isCompoundExp, isIfExp, AppExp, isAppExp, isNumExp, isBoolExp, isStrExp, isPrimOp, isVarRef, isVarDecl, LetExp , isLetExp, LitExp, parseDefine, DefineExp, Binding, VarDecl, isCExp, Program, Exp, isLetrecExp, LetrecExp, isSetExp, SetExp, CompoundExp, parseL4Exp} from './L4-ast'
import { Result, makeOk, makeFailure, mapResult } from '../shared/result';
import { map, isEmpty, bind, is } from 'ramda';
import { valueToString, CompoundSExp, Closure, isClosure } from './L4-value';
import { isEmptySExp, isCompoundSExp, isSymbolSExp, SExpValue, } from '../L3/L3-value';
import { isNumber, isString } from 'util';
import { isSexp } from '../shared/parser';
import parse, { Sexp, Token } from "s-expression";
import { isBinding, parseSExp } from '../L3/L3-ast';


let countif: number = 0;
let countApp: number = 0;
let countProc: number = 0;
let countLet: number = 0;
let countBool: number = 0;
let countNumber: number = 0;
let countBind: number = 0;
let countLit: number = 0;
let countProg: number = 0;
let countexpa=0;
let countprim=0;
let countvar=0;
let countstr=0;
let countvardecl=0;
let define=0;
let letrec=0;
let set=0;
let body=0;
let args=0;
let rands=0;
let compoumd=0;
let closoure=0;
let empty=0;
let symbol=0;
let params=0;
let number_small=0;
let str_small=0;
let prim_small=0;
let boolean_small=0;

export const makeIf = (v: string)  : string => {
    countif ++;
    return `${v}__${countif}`;
};

export const makeApp = (v: string)  : string => {
    countApp ++;
    return `${v}__${countApp}`;
};

export const makeProc = (v: string)  : string => {
    countProc ++;
    return `${v}__${countProc}`;
};

export const makeLet = (v: string)  : string => {
    countLet ++;
    return `${v}__${countLet}`;
};

export const makeBool = (v: string)  : string => {
    countBool ++;
    return `${v}__${countBool}`;
};

export const makeNumber = (v: string)  : string => {
    countNumber ++;
    return `${v}__${countNumber}`;
};

export const makeLit = (v: string)  : string => {
    countLit ++;
    return `${v}__${countLit}`;
};

export const makeBind = (v: string)  : string => {
    countBind ++;
    return `${v}__${countBind}`;
};

export const makeProg = (v: string)  : string => {
    countProg ++;
    return `${v}__${countProg}`;
};

export const makeexps= (v: string)  : string => {
    countexpa ++;
    return `${v}__${countexpa}`;
};

export const makestr= (v: string)  : string => {
    countstr ++;
    return `${v}__${countstr}`;
};

export const makeprim= (v: string)  : string => {
    countprim ++;
    return `${v}__${countprim}`;
};

export const makevar= (v: string)  : string => {
    countvar ++;
    return `${v}__${countvar}`;
};

export const makevardecl= (v: string)  : string => {
    countvardecl ++;
    return `${v}__${countvardecl}`;
};

export const makedefine= (v: string)  : string => {
    define ++;
    return `${v}__${define}`;
};

export const makeletrec= (v: string)  : string => {
    letrec ++;
    return `${v}__${letrec}`;
};

export const makeset= (v: string)  : string => {
    set ++;
    return `${v}__${set}`;
};

export const makebody= (v: string)  : string => {
    body ++;
    return `${v}__${body}`;
};

export const makeargs= (v: string)  : string => {
    args ++;
    return `${v}__${args}`;
};

export const makerands= (v: string)  : string => {
    rands ++;
    return `${v}__${rands}`;
};

export const makeclosure= (v: string)  : string => {
    closoure ++;
    return `${v}__${closoure}`;
};

export const makecompound= (v: string)  : string => {
    compoumd ++;
    return `${v}__${compoumd}`;
};

export const makeempty= (v: string)  : string => {
    empty ++;
    return `${v}__${empty}`;
};

export const makesymbol= (v: string)  : string => {
    symbol ++;
    return `${v}__${symbol}`;
};

export const makeparams= (v: string)  : string => {
    params ++;
    return `${v}__${params}`;
};
export const makesmall_number= (v: string)  : string => {
    number_small ++;
    return `${v}__${number_small}`;
};

export const booleam_small= (v: string)  : string => {
    boolean_small ++;
    return `${v}__${boolean_small}`;
};

export const small_prim =(v: string): string =>{
    prim_small ++;
    return `${v}__${prim_small}`; 
}

export const makestr_small =(v: string): string =>{
    str_small ++;
    return `${v}__${str_small}`; 
}

export const ParseGraphtoEdge = (exp: GraphContent[]) : Edge[] =>{
    let label= ""
    let args:Edge[]=exp.reduce(function(acc: Edge[], curr:GraphContent) {
        if(isCompoundGraph(curr)) {
            label= curr.edge[0].label ? label=curr.edge[0].label : ""
            return acc.concat([makeEdgeExp(makeNodeRef(curr.edge[0].from.id), curr.edge[0].to, label)].concat(curr.edge.slice(1,curr.edge.length)))
        }
        else{
            return acc
        }
    }, []);
    return args;
}

export const ParseGraph1toEdge = (exp: GraphContent) : Edge[] =>{
    let label= ""
    if(isCompoundGraph(exp)){
        exp.edge[0].label ? label=exp.edge[0].label : ""
    }
    return isCompoundGraph(exp) ? [makeEdgeExp(makeNodeRef(exp.edge[0].from.id), exp.edge[0].to, label)].concat(exp.edge.slice(1,exp.edge.length)): []
}

export const mapL4toMermaid = (exp: Parsed): Result<Graph> =>
    isProgram(exp) ? makeOk(makeGraph(makeHeaderExp("TD"),ParseProgram(exp))) :
    isCExp(exp) ? makeOk(makeGraph(makeHeaderExp("TD"),ParseCexp(exp))) : 
    isDefineExp(exp) ? makeOk(makeGraph(makeHeaderExp("TD"),ParseDefine(exp)))  :
    makeFailure("Error");


export const ParseProgram =(exp:Program) : GraphContent =>{
    let name= makeProg("Program")
  //  let vars:GraphContent[]= exp.exps.map((x) => ParseCexp(x))
    let body:GraphContent[]= exp.exps.map((x) => ParseCexp(x))
    let name_exps= makeexps("Exps")
    let body_insert:Edge[]=body.reduce( (acc: Edge[], curr:GraphContent)=> {
        return isCompoundGraph(curr) ? acc.concat(makeEdgeExp(makeNodeRef(name_exps),  curr.edge[0].from )) :
        acc.concat(makeEdgeExp(makeNodeRef(name_exps), curr.val))
    }, []);
    return makeCompoundGraph([makeEdgeExp(makeNodeDecl(name,exp.tag), makeNodeDecl(name_exps,":"), "exps")].concat(body_insert).concat(ParseGraphtoEdge(body)))
     
}   

    
export const ParseCexp = (exp: Exp) : GraphContent =>
    isProcExp(exp) ? ParsePrc(exp) :
    isIfExp(exp) ? ParseIf(exp) :
    isAppExp(exp) ? ParseApp(exp) :
    isBoolExp(exp) ? makeAtomicGraph(makeNodeDecl(makeBool("BoolExp"), (exp) ? `"BoolExp(#t)"`: `"BoolExp(#f)"`)) :
    isNumExp(exp) ? makeAtomicGraph(makeNodeDecl(makeNumber("NumExp"), `"NumExp(${exp.val})"`)) :
    isStrExp(exp) ? makeAtomicGraph(makeNodeDecl(makestr("StrExp"), `"StrExp(${exp.val})"`)) :
    isPrimOp(exp) ? makeAtomicGraph(makeNodeDecl(makeprim("PrimOp"),`"PrimOp(${exp.op})"`)):
    isVarRef(exp) ? makeAtomicGraph(makeNodeDecl(makevar("VarRef"),`"VarRef(${exp.var})"`)) :
    isLetExp(exp) ? ParseLet(exp) :
    isDefineExp(exp) ? ParseDefine(exp):
    isLetrecExp(exp) ? ParseLetRec(exp):
    isSetExp(exp) ? ParseSet(exp) :
    isLetExp(exp) ? ParseLet(exp) :
    ParseLit(exp)

export const ParseVar =(exp: VarDecl) : GraphContent =>
     makeAtomicGraph(makeNodeDecl(makevardecl("VarDecl"), `"VarDecl(${exp.var})"`))
    

export const ParseDefine =(exp: DefineExp) : GraphContent => {
   let name=makedefine(exp.tag)
   let body: GraphContent= ParseCexp(exp.val)
   let vardecl: GraphContent =ParseVar(exp.var)
   return makeCompoundGraph([(makeEdgeExp(makeNodeDecl(name, "DefineExp"), isCompoundGraph(vardecl) ? vardecl.edge[0].from : vardecl.val, "var")) ,makeEdgeExp(makeNodeRef(name), isCompoundGraph(body) ? body.edge[0].from : body.val, "val")].concat(ParseGraph1toEdge(vardecl)).concat(ParseGraph1toEdge(body))) 
} 

export const ParseClosure = (exp:Closure) : GraphContent =>{
    let name=makeProc(exp.tag);
    let args_name= makeargs("Args")
    let body_name= makebody("Body")
    let args:Edge[]=exp.params.reduce( (acc: Edge[], curr:VarDecl)=> {
        return acc.concat(makeEdgeExp(makeNodeRef(args_name), makeNodeDecl(makevardecl('VarDecl'), `VarDecl(${curr.var}`)))
    }, []);
    let body:GraphContent[]= exp.body.map((x) => ParseCexp(x))
    let body_insert:Edge[]=body.reduce( (acc: Edge[], curr:GraphContent)=> {
        return isCompoundGraph(curr) ? acc.concat(makeEdgeExp(makeNodeRef(body_name),  curr.edge[0].from )) :
        acc.concat(makeEdgeExp(makeNodeRef(makebody(body_name)), curr.val))
    }, []);
    return makeCompoundGraph([makeEdgeExp(makeNodeDecl(name, exp.tag), makeNodeDecl(args_name, ":"), "args"), makeEdgeExp(makeNodeRef(name), makeNodeDecl(body_name, ":"), "body")].concat(args).concat(body_insert).concat(ParseGraphtoEdge(body))); 
}

export const ParseLetRec = (exp: LetrecExp) : GraphContent =>{
    let name=makeletrec(exp.tag);   
    let vars:GraphContent[]= exp.bindings.map((x) => ParseBindings(x, makeBind("Bindings")))
    let bindings_name= isCompoundGraph(vars[0]) ? vars[0].edge[0].from.id : vars[0].val.id
    let bindings:Edge[]=vars.reduce( (acc: Edge[], curr:GraphContent)=> {
     //   let bindings_name1= makeBind("Bindings");
        return isCompoundGraph(curr) ? acc.concat(makeEdgeExp(makeNodeRef(bindings_name),  curr.edge[0].from )) :
        acc.concat(makeEdgeExp(makeNodeRef(bindings_name), curr.val))
    }, []);
    let body:GraphContent[]= exp.body.map((x) => ParseCexp(x))
    let body_name= makebody("Body")
    let body_insert:Edge[]=body.reduce( (acc: Edge[], curr:GraphContent)=> {
        return isCompoundGraph(curr) ? acc.concat(makeEdgeExp(makeNodeRef(body_name),  curr.edge[0].from )) :
        acc.concat(makeEdgeExp(makeNodeRef(body_name), curr.val))
    }, []);
    return makeCompoundGraph([makeEdgeExp(makeNodeDecl(name, exp.tag), makeNodeDecl(bindings_name, ":"), "bindings"), makeEdgeExp(makeNodeRef(name), makeNodeDecl(body_name, ":"), "body"),].concat(body_insert).concat(bindings).concat(ParseGraphtoEdge(vars)).concat(ParseGraphtoEdge(body)));
}


export const ParseSet = (exp: SetExp) : GraphContent => {
    let name=makeset(exp.tag);
    let body: GraphContent = ParseCexp(exp.val)
    let varref: GraphContent = ParseCexp(exp.var)
    return makeCompoundGraph([makeEdgeExp(makeNodeDecl(name, exp.tag), isCompoundGraph(varref) ? varref.edge[0].from : varref.val, "Var"),
    makeEdgeExp(makeNodeRef(name), isCompoundGraph(body) ? body.edge[0].from : body.val, "Val")].concat(ParseGraph1toEdge(varref).concat(ParseGraph1toEdge(body))))
}

export const ParseLet =(exp: LetExp) :GraphContent =>{
    let name=makeLet(exp.tag);   
    let vars:GraphContent[]= exp.bindings.map((x) => ParseBindings(x, makeBind("Bindings")))
    let bindings_name= isCompoundGraph(vars[0]) ? vars[0].edge[0].from.id : vars[0].val.id
    let bindings:Edge[]=vars.reduce( (acc: Edge[], curr:GraphContent)=> {
     //   let bindings_name1= makeBind("Bindings");
        return isCompoundGraph(curr) ? acc.concat(makeEdgeExp(makeNodeRef(bindings_name),  curr.edge[0].from )) :
        acc.concat(makeEdgeExp(makeNodeRef(bindings_name), curr.val))
    }, []);
    let body:GraphContent[]= exp.body.map((x) => ParseCexp(x))
    let body_name= makebody("Body")
    let body_insert:Edge[]=body.reduce( (acc: Edge[], curr:GraphContent)=> {
        return isCompoundGraph(curr) ? acc.concat(makeEdgeExp(makeNodeRef(body_name),  curr.edge[0].from )) :
        acc.concat(makeEdgeExp(makeNodeRef(body_name), curr.val))
    }, []);
    return makeCompoundGraph([makeEdgeExp(makeNodeDecl(name, exp.tag), makeNodeDecl(bindings_name, ":"), "bindings"), makeEdgeExp(makeNodeRef(name), makeNodeDecl(body_name, ":"), "body"),].concat(body_insert).concat(bindings).concat(ParseGraphtoEdge(vars)).concat(ParseGraphtoEdge(body)));
}
 
export const ParseBindings = (exp:Binding, name:string) : GraphContent =>{
  //  let name=makeBind("Bindings");
    let val_name = makevardecl("VarDecl");
    let bodyc: GraphContent= ParseCexp(exp.val)
     let body_insert: Edge= isCompoundGraph(bodyc) ? (makeEdgeExp(makeNodeRef(name),  bodyc.edge[0].from, 'val' )) :
     makeEdgeExp(makeNodeRef(name), bodyc.val, 'val')
     return makeCompoundGraph([makeEdgeExp(makeNodeDecl(name, "Bindings"), makeNodeDecl(val_name, `"VarDecl(${exp.var.var})"`), "var")].concat(body_insert).concat(ParseGraph1toEdge(bodyc)));
}


export const ParseLit =(exp: LitExp) : GraphContent => {
    let name=makeLit(exp.tag);
    if(isEmptySExp(exp.val)){
     return makeCompoundGraph([makeEdgeExp(makeNodeDecl(name, exp.tag), (makeNodeDecl(makeempty("Empty"), "EmptyExp")), "val")]) 
    }
    if(isCompoundSExp(exp.val))
    { 
        let namecompound= makecompound("CompoundSExp");
        return makeCompoundGraph([makeEdgeExp(makeNodeDecl(name, exp.tag), makeNodeDecl(namecompound, `"CompoundSExp"`), "val")].concat(ParseGraph1toEdge(makeCompoundGraph(ParseCompound(exp.val, namecompound)))));
    }
    if(isNumber(exp.val)) return makeCompoundGraph([makeEdgeExp(makeNodeDecl(name, exp.tag),(makeNodeDecl(makeNumber("NumExp"),`"number(${exp.val})"`)), "val")])
    if(isPrimOp(exp.val)) return makeCompoundGraph([makeEdgeExp(makeNodeDecl(name, exp.tag),(makeNodeDecl(makeprim("PrimExp"),`"primop(${exp.val})"`)), "val")])
    if(isBoolExp(exp.val)) return makeCompoundGraph([makeEdgeExp(makeNodeDecl(name, exp.tag),(makeNodeDecl(makeBool("BoolExp"),`"boolean(${exp.val})"`)), "val")])
    if(isString(exp.val)) return makeCompoundGraph([makeEdgeExp(makeNodeDecl(name, exp.tag),(makeNodeDecl(makestr("StringExp"),"String_Exp")), "val")])
    if(isSymbolSExp(exp.val)) return makeCompoundGraph([makeEdgeExp(makeNodeDecl(name,exp.tag), (makeNodeDecl(makesymbol("SymbolExp"), `"symbol(${exp.val})"`)), "val")]) 
    else if (isClosure(exp.val)) {
        let value:GraphContent= ParseClosure(exp.val)
        return makeCompoundGraph([makeEdgeExp(makeNodeDecl(name, exp.tag),isCompoundGraph(value) ? value.edge[0].from : value.val, "val")])
    }
    return makeCompoundGraph([])
}

export const ParseCompound = (exp: SExpValue, name: string) : Edge[] => {
    if(isCompoundSExp(exp)){
       let val1: GraphContent=ParseSExp(exp.val1, name);
       let val2: GraphContent=ParseSExp(exp.val2, name);
         return [makeEdgeExp(makeNodeDecl(name, `"CompoundSExp"`), isCompoundGraph(val1) ? val1.edge[0].from : val1.val, "val1"),makeEdgeExp(makeNodeRef(name), isCompoundGraph(val2) ? val2.edge[0].from : val2.val, "val2")].concat(ParseGraph1toEdge(val1).concat(ParseGraph1toEdge(val2)))
    }
    else
    return []
}

export const ParseSExp = (exp:SExpValue, name:string) : GraphContent =>{
    if(isEmptySExp(exp)){
        return makeAtomicGraph(makeNodeDecl(makeempty("EmptySExp"), `"EmptyExp"`))
       }
       if(isCompoundSExp(exp))
       { 
           let namecompound= makecompound("CompoundSExp");
           return makeCompoundGraph(ParseCompound(exp, namecompound));
       }
       if(isNumber(exp)) return makeAtomicGraph(makeNodeDecl(makeNumber("NumExp"), `"number(${exp})"`))
       if(isPrimOp(exp)) return makeAtomicGraph(makeNodeDecl(makeprim("PrimExp"), `"primop(${exp})"`))
       if(isBoolExp(exp)) return makeAtomicGraph(makeNodeDecl(makeBool("BoolExp"), `"boolean(${exp})"`))
       if(isString(exp)) return makeAtomicGraph(makeNodeDecl(makestr("StrExp"), `"string(${exp})"`))
       if(isSymbolSExp(exp)) return makeCompoundGraph([makeEdgeExp(makeNodeDecl(name,exp.tag), (makeNodeDecl(makesymbol("SymbolExp"), `"symbol(${exp.val})"`)), "val")]) 
       return makeCompoundGraph([])
}

export const ParseIf = (exp: IfExp) : GraphContent =>{
    let name=makeIf(exp.tag);
    let body: CExp[] = [exp.test, exp.then, exp.alt];
    let body_parse: GraphContent[] = body.map((x) => ParseCexp(x));
    let value: CompoundGraph=makeCompoundGraph([makeEdgeExp(makeNodeDecl(name, exp.tag), isCompoundGraph(body_parse[0]) ? body_parse[0].edge[0].from: body_parse[0].val, "test"),
    makeEdgeExp(makeNodeRef(name), isCompoundGraph(body_parse[1]) ? body_parse[1].edge[0].from : body_parse[1].val, "then"),
    makeEdgeExp(makeNodeRef(name), isCompoundGraph(body_parse[2]) ? body_parse[2].edge[0].from : body_parse[2].val, "alt")].concat(ParseGraphtoEdge(body_parse)))
    return value
}


export const ParseApp = (exp: AppExp) : GraphContent =>{
    let name=makeApp(exp.tag);
    let body_parse: GraphContent[] = exp.rands.map((x) => ParseCexp(x)); 
    let rator: GraphContent= ParseCexp(exp.rator);
    let rands: string=makerands("Rands")
    let body_insert:Edge[]=body_parse.reduce( (acc: Edge[], curr:GraphContent)=> {
        return isCompoundGraph(curr) ? acc.concat(makeEdgeExp(makeNodeRef(rands),  curr.edge[0].from )) :
        acc.concat(makeEdgeExp(makeNodeRef(rands), curr.val))
    }, []);
    return makeCompoundGraph([makeEdgeExp(makeNodeDecl(name,exp.tag), isCompoundGraph(rator) ? rator.edge[0].from : rator.val, "rator"), makeEdgeExp(makeNodeRef(name), makeNodeDecl(rands, ":"), "rands")].concat(ParseGraph1toEdge(rator)).concat(body_insert).concat(ParseGraphtoEdge(body_parse)))
}

export const ParsePrc = ( exp: ProcExp) : GraphContent => {
    let name=makeProc(exp.tag);
    let args_name= makeparams("Params")
    let body_name= makebody("Body")
    let args:Edge[]=exp.args.reduce( (acc: Edge[], curr:VarDecl)=> {
        return acc.concat(makeEdgeExp(makeNodeRef(args_name), makeNodeDecl(makevardecl('VarDecl'), `"VarDecl(${curr.var})"`)))
    }, []);
    let body:GraphContent[]= exp.body.map((x) => ParseCexp(x))
    let body_insert:Edge[]=body.reduce( (acc: Edge[], curr:GraphContent)=> {
        return isCompoundGraph(curr) ? acc.concat(makeEdgeExp(makeNodeRef(body_name),  curr.edge[0].from )) :
        acc.concat(makeEdgeExp(makeNodeRef(body_name), curr.val))
    }, []);
    return makeCompoundGraph([makeEdgeExp(makeNodeDecl(name, exp.tag), makeNodeDecl(args_name, ":"), "args"), makeEdgeExp(makeNodeRef(name), makeNodeDecl(body_name, ":"), "body")].concat(body_insert).concat(args).concat(ParseGraphtoEdge(body)));
}

