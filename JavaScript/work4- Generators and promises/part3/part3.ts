import {chain, map, zipWith } from "ramda";


function* braid(generator1: () => Generator, generator2: () => Generator): Generator { 
    let val=0
    let it = generator1();
    let it2 = generator2();
    while(true){
        if((val%2 ==0) || (generator2().next().done)){
            yield it.next().value
        }
        else if(((val%2 !=0) || (generator1().next().done))){
            yield it2.next().value
        }
     val++
    }
}

function* biased(generator1: () => Generator, generator2: () => Generator): Generator { 
    let val=2
    let it = generator1();
    let it2 = generator2();
    while(true){
        if((val > 0) || (generator2().next().done)){
           val--; 
           yield it.next().value
        }
        else if(((val < 2) || (generator1().next().done))){
           val++
           yield generator2().next().value
        }
    }
}

 function take(n: Number, g:Generator) { 
     const result = []; 
     for (let i = 0; i < n; i++) { 
     const { value, done } = g.next(); 
     if (done) { 
     break; 
     }
    result.push(value);
     } 
     return result; 
 } 

function* gen1() {
    yield 3;
    yield 6;
    yield 9;
    yield 12;
}
function* gen2() {
    yield 8;
    yield 10;
}
