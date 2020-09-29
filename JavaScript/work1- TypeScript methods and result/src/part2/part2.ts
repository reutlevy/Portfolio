

            import { any, Pred, map, filter, chain, compose, not, reduce, T, reject, concat, pipe } from "ramda";

                /* Question 1 */
                export const partition :<T> (f:Pred, a: T[]) => T[][]=
                <T>(f: Pred, a: T[]) : T[][] => {
                return [a.filter(f), reject(f,a)];
                }
                

                /* Question 2 */
                export const mapMat :<T> (f: (y:T) => T, a: T[][]) => T[][] =
                <T>(f: (y:T) => T, a: T[][]) : T[][] => a.map(function(array) {
                    return array.map(f)
                });
                

                /* Question 3 */
                export const composeMany :<T> (f: ((y:T) => T) []) =>  ((y:T) => T) =
                    <T> (f: ((y:T) => T) []) : ((y:T) => T ) => 
                    f.reduce((curr, next) => compose(curr,next));


                /* Question 4 */
                interface Languages {
                    english: string;
                    japanese: string;
                    chinese: string;
                    french: string;
                }

                interface Stats {
                    HP: number;
                    Attack: number;
                    Defense: number;
                    "Sp. Attack": number;
                    "Sp. Defense": number;
                    Speed: number;
                }

                interface Pokemon {
                    id: number;
                    name: Languages;
                    type: string[];
                    base: Stats;
                }


                export const maxSpeed :(array: Pokemon []) => Pokemon[] =
                (array: Pokemon []) :Pokemon[] => {
                let max:Number=Math.max.apply(null, array.map(x=> x.base).map(y=> y.Speed));
                let arr:Pokemon[]=array.reduce(function (acc: Pokemon[], curr:Pokemon) {
                    if (curr.base.Speed== max) { return acc.concat(curr) }
                    return acc;
                }, []);
                return arr; }
        

                export const grassTypes = (arrayP:Pokemon[])=>{
                    let PokGrass= arrayP.filter((p:Pokemon)=>p.type.filter((y:string)=>y==="Grass" ).length>0);
                    return PokGrass.reduce((names:String[],p)=>{return names.concat(p.name.english);},[]).sort();
                };

                
                export const uniqueTypes = (arrayP:Pokemon[])=>{
                    let pTypes= arrayP.reduce((acc:string[],p:Pokemon)=>acc.concat(p.type),[]);
                    return pTypes.filter((t:string,a)=> pTypes.indexOf(t)===a).sort();};
  
              
