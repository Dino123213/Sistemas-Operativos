
sig Persona {} 

one sig Gimnasio {
    ocupantes: set Persona,
    capacidad: one Int
}

// Limitando la cantidad de personas: en toda instancia hay mas personas que capacidad
fact{#Persona > Gimnasio.capacidad}
// Definición de la capacidad del gimnasio
fact{ Gimnasio.capacidad = 5}

// Predicado para cuando una Persona entra al gimnasio
pred entrar[p: Persona, g1, g2: Gimnasio] {
    p in g2.ocupantes and
    (g1.ocupantes - p) = g2.ocupantes - p

}

fun personasOcupando[]: set Persona{
	{p: Persona | p in Gimnasio.ocupantes}
}

//Verifico si es posible definir la signatura gimnasio como unitaria.
run {} for 6
/*
Resultado: Genera instancias
┌─────────────┬─────────┬─────────┐
│this/Gimnasio│ocupantes│capacidad│
├─────────────┼─────────┼─────────┤
│Gimnasio$0   │Persona$5│5        │
└─────────────┴─────────┴─────────┘
Analisis: El modelo permite generar instancias aunque gimnasio este definido como one
Objetivo: Verificar que el modelo permita generar instancias con el gimnasio siendo one sig */


run entrarGimnasio{some p:Persona, g1,g2:Gimnasio | entrar[p,g1,g2]} for 10
/* Resultado: Genera instancias
   entrar[p,g1,g2]== g1=g2 and entrar[p,g1,g2]
   Analisis: El predicado puede funcionar pero no modela el comportamiento normal, ya que toma a g1
y g2 como el mismo gimnasio, ya que por cada instancia generada se tiene una cantidad distinta de ocupantes
pero siempre siendo ocupantes del mismo gimnasio, ya que esta declarado como one sig Gimnasio{}, mientras que lo que deberia suceder
es la creacion de diversos atomos de gimnasios debido a la entrada de ocupantes.
Objetivo: Verificar si el predicado entrar se puede usar con un solo gimnasio
*/

run entrarGimnasioV2{some p:Persona, g1,g2:Gimnasio | g1!=g2 and entrar[p,g1,g2]} for 10
/* Resultado: No genera instancias
Analisis: No genera instancias ya que al declarar gimnasio como one sig Gimnasio{}, esto hace que 
sea imposible generar una entrada de un ocupante debido a que no puede crearse otro atomo de Gimnasio para modelar la entrada
del ocupante.
Objetivo: Verificar si el predicado entrar se puede usar en dos atomos de gimnasio distintos
Conclusion: No es posible definir la signatura Gimnasio como unitaria ya que al intentar modelar dinamica no se puede tener un solo atomo de Gimnasio, esto no tiene sentido
y rompe con el propositio de que distintos atomos de gimnasio representen al mismo para asi poder aplicar funciones, como entrar la cual mediante dos atomos de gimnasio*/
