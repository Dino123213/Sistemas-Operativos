
sig Persona {} 

sig Gimnasio {
    ocupantes: set Persona,
    capacidad: one Int
}

// Limitando la cantidad de personas: en toda instancia hay mas personas que capacidad
fact{#Persona > Gimnasio.capacidad}
// Definición de la capacidad del gimnasio
fact{ Gimnasio.capacidad = 5}

//Facts del enunciado

//El fact indica que para todo gimnasio g el numero de atomos dentro de g.ocupantes es menor al entero de Gimnasio.capacidad, en este caso 5
fact capacidad1{ all g:Gimnasio | #g.ocupantes <= Gimnasio.capacidad}

//El fact indica que el numero de atomos dentro Gimnasio.ocupantes es menor o igual al entero de Gimnasio.capacidad, en este caso 5
fact capacidad2{#Gimnasio.ocupantes <= Gimnasio.capacidad}

// Predicado para cuando una Persona entra al gimnasio
pred entrar[p: Persona, g1, g2: Gimnasio] {
    p in g2.ocupantes and
    (g1.ocupantes - p) = g2.ocupantes - p
}


run sinRestricciones{} 
/* 
Resultado: No genera instancias
Analisis: No genera instancias ya que el modelado necesita al menos 6 atomos de persona
y al no tener scope el scope predeterminado es 3, por lo tanto solo puede generar 3 atomos de persona.
Objetivo: Verificar que no haya ninguna necesidad de scope minimo o cambio de bitwith.
*/

run conScope{} for 6
/* 
Resultado: Genera instancias
Analisis: Genera instancias ya que el modelado necesita al menos 6 atomos de persona
y al poner scope 6, ya puede generar los 6 atomos de persona que necesitaba.
Objetivo: Verificar que no haya ninguna necesidad de cambio de bitwith.
*/


/**Comentando fact "fact  capacidad2{#Gimnasio.ocupantes <= Gimnasio.capacidad}"  **/

run chequeoFactsV1{some disj g1,g2:Gimnasio |  
#(g1.ocupantes)=3 and #(g2.ocupantes)=3 and g2.ocupantes&g1.ocupantes=none}for 10

/* Al comentar el fact 2 podemos observar que:
Resultado: Genera instancias
┌─────────────┬─────────┬─────────┐
│this/Gimnasio│ocupantes│capacidad│
├─────────────┼─────────┼─────────┤
│Gimnasio$0   │Persona$3│5        │
│             ├─────────┼─────────┤
│             │Persona$4│         │
│             ├─────────┤         │
│             │Persona$5│         │
├─────────────┼─────────┼─────────┤
│Gimnasio$1   │Persona$0│5        │
│             ├─────────┼─────────┤
│             │Persona$1│         │
│             ├─────────┤         │
│             │Persona$2│         │
└─────────────┴─────────┴─────────┘
Analisis: Como podemos observar el run genera instancias, el modelado comentando el fact permite 
que hayan 5 personas distintas las cuales esten inscriptos cada uno a un solo gimnasio, por lo tanto este 
fact es correcto y modela correctamente la situacion planteada en el texto.
Objetivo: Verificar que el fact capacidad1 este funcionando correctamente
*/

/**Comentando fact "fact capacidad1{ all g:Gimnasio | #g.ocupantes <= Gimnasio.capacidad}" o
    no comentando ninguno de los facts **/

run chequeoFactsV2{some disj g1,g2:Gimnasio |  
#(g1.ocupantes)=3 and #(g2.ocupantes)=3 and g2.ocupantes&g1.ocupantes=none}for 10

/* Al comentar el fact 1 o no comentarlo podemos observar que:
Resultado: No genera instancias
Analisis: Esto se debe a que el fact no permite que el array final de ocupantes del gimnasio sea mayor a la capacidad
de los gimnasios la cual es 5, al decirle que la cantidad de ocupantes es 3 en un gimnasio y 3 en otro implica
que la cantidad total de personas distintas son 6, los cuales son ocupantes y son los que no van a permitir generar
instancias Gimnasio.ocupantes<=Gimnasio.capacidad.
Objetivo: Verificar que el fact capacidad 2 este funcionando correctamente.
*/
