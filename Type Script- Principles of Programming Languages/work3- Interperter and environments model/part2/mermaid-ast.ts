// ===========================================================
// AST type models
import { map, zipWith, is, reduce, head } from "ramda";
import parse, { Sexp, Token } from "s-expression";
import { allT, first, second, rest, isEmpty } from "../shared/list";
import { isArray, isString, isNumericString, isIdentifier } from "../shared/type-predicates";
import { parse as p, isSexpString, isToken } from "../shared/parser";
import { Result, makeOk, makeFailure, bind, mapResult, safe2 } from "../shared/result";
import { isSymbolSExp, isEmptySExp, isCompoundSExp } from './L4-value';
import { makeEmptySExp, makeSymbolSExp, SExpValue, makeCompoundSExp, valueToString } from './L4-value'
import { isBoolExp, isNumExp, isStrExp, isLitExp, isVarRef, isProcExp, isIfExp, isAppExp, isPrimOp, Exp, parseL4Exp, parseL4Program, Program } from "./L4-ast";
import { mapL4toMermaid } from "./mermaid";

/*
;; =============================================================================

<graph> ::= <header> <graphContent> // Graph(dir: Dir, content: GraphContent)
<header> ::= graph (TD|LR)<newline> // Direction can be TD or LR
<graphContent> ::= <atomicGraph> | <compoundGraph>
<atomicGraph> ::= <nodeDecl>
<compoundGraph> ::= <edge>+
<edge> ::= <node> --><edgeLabel>? <node><newline> // <edgeLabel> is optional
// Edge(from: Node, to: Node, label?: string)
<node> ::= <nodeDecl> | <nodeRef>
<nodeDecl> ::= <identifier>["<string>"] // NodeDecl(id: string, label: string)
<nodeRef> ::= <identifier> // NodeRef(id: string)
<edgeLabel> ::= |<identifier>| // string

;; =============================================================================
*/



// a value rerturned by graph
export type GraphContent= AtomicGraph | CompoundGraph;
export type Node= NodeDecl | NodeRef;

export interface Graph {tag: "Graph"; header: Header; content: GraphContent;}
export interface Header {tag: "Header"; val: "TD"|"LR"; }
export interface AtomicGraph {tag: "AtomicGraph"; val: NodeDecl; }
export interface CompoundGraph {tag: "CompoundGraph"; edge: Edge[]; }
export interface Edge {tag: "Edge"; from:Node; to:Node; label?:string;}
export interface NodeDecl {tag: "NodeDecl"; id: string; label: string; }
export interface NodeRef {tag: "NodeRef"; id: string; }
export interface EdgeLabel {tag: "EdgeLabel"; id: string; }

// Type value constructors for disjoint types
export const makeGraph= (header: Header, content: GraphContent): Graph => ({tag: "Graph", header: header, content: content});
export const makeHeaderExp= (val:"TD"|"LR"): Header=> ({tag: "Header", val: val,});
export const makeAtomicGraph= (val: NodeDecl): AtomicGraph => ({tag: "AtomicGraph", val: val});
export const makeCompoundGraph= (edge: Edge[]): CompoundGraph=> ({tag: "CompoundGraph", edge: edge});
export const makeEdgeExp= (from:Node, to:Node, label?:string): Edge=> ({tag: "Edge", from:from, to:to , label:label});
export const makeNodeDecl= (id: string, label: string): NodeDecl=> ({tag: "NodeDecl", id: id, label: label});
export const makeNodeRef= (id: string): NodeRef=> ({tag: "NodeRef", id:id});
export const makeEdgeLabel= (id: string): EdgeLabel=> ({tag: "EdgeLabel", id: id});

// Type predicates for disjoint types
export const isGraph= (x: any): x is Graph => x.tag === "Graph";
export const isHeaderExp = (x: any): x is Header => x.tag === "Header";
export const isAtomicGraph = (x: any): x is AtomicGraph => x.tag === "AtomicGraph";
export const isCompoundGraph = (x: any): x is CompoundGraph => x.tag === "CompoundGraph";
export const isEdgeExp = (x: any): x is Edge => x.tag === "Edge";
export const isNodeDecl = (x: any): x is NodeDecl => x.tag === "NodeDecl";
export const isNodeRef = (x: any): x is NodeRef => x.tag === "NodeRef";
export const isEdgeLabel = (x: any): x is EdgeLabel => x.tag === "EdgeLabel";

// Type predicates for type unions
export const isGraphContent= (x: any): x is GraphContent => isAtomicGraph(x) || isCompoundGraph(x);
export const isNode = (x: any): x is Node => isNodeDecl(x) || isNodeRef(x) ;


export const unparseMermaid = (exp: Graph): Result<string> =>
     isAtomicGraph(exp.content) ? makeOk(`graph ${exp.header.val} ${parseAtomic(exp.content)}`):
     isCompoundGraph(exp.content) ? makeOk(`graph ${exp.header.val} ${parseCompound(exp.content)}`):
     makeFailure("Error")


export const parseAtomic = (exp: AtomicGraph): string =>
    `\n${(exp.val.id)}[${(exp.val.label)}]\n`

export const parseCompound = (exp: CompoundGraph) : string => {
     let result="";
     for (let i=0; i<exp.edge.length ; i++){
          result=result+"\n"+parseEdges(exp.edge[i])
     }
     return result+"\n";
}

export const parseEdges = (exp: Edge) : string => {
     let label1= exp.label ? `|${exp.label}|` : ""
     return `${ isNodeDecl(exp.from) ? parse_Decl(exp.from): exp.from.id} -->${label1} ${isNodeDecl(exp.to) ? parse_Decl(exp.to): exp.from.id}` 
}

export const parse_Decl = (exp : NodeDecl) : string =>{
    let result: string= `${ exp.id}[${exp.label}]` 
    return result; 
}

export const L4toMermaid = (concrete: string): Result<string> =>{
     if(concrete.includes("L4"))
         return bind(parseL4Program(parse(concrete)), (val: Program) => L4toString(val))
     return bind(parseL4Exp(parse(concrete)), (val: Exp) => L4toString(val))
}

export const L4toString = (exp: Exp | Program) : Result<string> =>  
     bind(mapL4toMermaid(exp), (val: Graph) => unparseMermaid(val))


