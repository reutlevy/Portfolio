
    const f = (x: number): Promise<number> =>
    new Promise<number>((resolve, reject) => {
        if (x==0) {
          reject("Cant divide by 0");
        } else {
        resolve(1/x);
    }
    });
   
    const g = (x: number): Promise<number> =>
    new Promise<number>((resolve) => {
        resolve(x*x);
    });

    const h = (x: number): Promise<number> =>
    new Promise<number>((resolve, reject) => {
        g(x)
        .then((data) => {
              f(data)
              .then((value)=> {
                  console.log(value)
                   resolve(value)
              })
              .catch((error) => reject(error)) 
        })
        .catch((error) => reject(error))       
    });

    const slower = <T>(p1: Promise<T>[]): Promise<string> =>
    new Promise<string>((resolve, reject) => {
        p1[0]
        .then((data) => {
           p1[1]
            .then((value)=> {
                 resolve(`(1 ${data})`)
            })
            .catch((error) => reject(error)) 
      })
      .catch((error) => reject(error))
      p1[1]
      .then((data) => {
         p1[0]
          .then((value)=> {
               resolve(`(0 ${value})`)
          })
          .catch((error) => reject(error)) 
    })
    .catch((error) => reject(error))     
    });






