/* Question 3 */
import {compose, reduce} from "ramda"
export type Result<T> = Ok<T>|Failure<T>;

interface Ok<T> {
    tag: "Ok";
    value: T;
 }

 interface Failure<T> {
    tag: "Failure";
    message:string;
 }

export const makeOk = <T>(y:T) : Result<T> =>
({tag: "Ok", value: y});

export const makeFailure = <T>(y:string) : Result<T> =>
({tag: "Failure", message: y});;

export const isOk = <T>(x:Result<T>): x is Ok<T> => x.tag === "Ok";
export const isFailure = <T>(x: Result<T>): x is Failure<T> => x.tag === "Failure";

/* Question 4 */
export const bind =  <T, U>(Result: Result<T>, f: (x: T) => Result<U>): Result<U> => { 
    if(isOk(Result)) {
        const newResult = f(Result.value);
        return newResult;
    }
    else {
        return Result;
    }
};

/* Question 5 */
interface User {
    name: string;
    email: string;
    handle: string;
}


const validateName = (user: User): Result<User> =>
    user.name.length === 0 ? makeFailure("Name cannot be empty") :
    user.name === "Bananas" ? makeFailure("Bananas is not a name") :
    makeOk(user);

const validateEmail = (user: User): Result<User> =>
    user.email.length === 0 ? makeFailure("Email cannot be empty") :
    user.email.endsWith("bananas.com") ? makeFailure("Domain bananas.com is not allowed") :
    makeOk(user);

const validateHandle = (user: User): Result<User> =>
    user.handle.length === 0 ? makeFailure("Handle cannot be empty") :
    user.handle.startsWith("@") ? makeFailure("This isn't Twitter") :
    makeOk(user);


export const naiveValidateUser = (user:User) : Result<User> => {
   let result: Result<User>= validateName(user);
   (isFailure(result)) ? makeFailure(result.message):
   result=validateEmail(user);
   (isFailure(result)) ? makeFailure(result.message):
   result=validateHandle(user);
   (isFailure(result)) ? makeFailure(result.message):
   result=makeOk(user);
   return result;
}

export const monadicValidateUser = (user:User): Result<User>=>  {
      return reduce(bind, validateName(user), [validateEmail, validateHandle]);
}



     