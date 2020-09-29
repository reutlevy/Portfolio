/* Question 1 */

import {T, none, F} from "ramda";

export type Optional<T> = Some<T>|None<T>;

interface Some<T> {
    tag: "Some";
    value: T;
 }

 interface None<T> {
    tag: "None";
 }
    
export const makeSome= <T>(y:T) : Optional<T> =>
({tag: "Some", value: y});


export const makeNone = <T>() : Optional<T> =>
({tag: "None"});

export const isSome = <T>(x: Optional<T>): x is Some<T> => x.tag === "Some";
export const isNone = <T>(x: Optional<T>): x is None<T> => x.tag === "None";


/* Question 2 */
export const bind = <T, U>(option: Optional<T>, f: (x: T) => Optional<U>): Optional<U> => {  
      if(isNone(option)){
        return option;
      }
      
      else{
        const newoptional:Optional<U>=f(option.value);
        return newoptional;
      }
};
