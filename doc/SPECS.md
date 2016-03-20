# Artecshpp specs

<small>**This document is in alpha stage and will constantly evolve during development.**</small>

## Table of Contents

1. [What is an ECS?](#1-what-is-an-ecs)
2. [Goals](#2-goals)
3. [System overview](#3-system-overview)
  1. [Aspects](#3-1-aspects)
  2. [Entity filtering](#3-2-entity-filtering)

---

## 1. What is an ECS?

A lot has been written related to Entity-Component systems focused on games, so I will barely sketch the concept. The core idea at ECS is to avoid inheritance on behalf of composition.

#### Components
A **component** is a piece of data representing some aspect of an entity that makes sense in our defined game world. Examples are position, the ability to jump or to fly.

#### Entities
An **entity** is a set of closely related components making sense together. For example, an enemy shooting bird in a 2D platformer could be an entity with components like flying, shooting, position, animation and the ability of seeking the player.  

#### Systems
Finally, we have **systems**, which performs logic on one or several components of an entity. A system will act on an entity only if it contains certain components. For example, a movement system would could act in an entity if it has a position component **and** a speed component.

Noticed I remarked the **and** word? this is because we will later use a higher level concept, so that we can add more complex conditions to accept an entity in a system, this is, checking if the entity does **not** have certain component, or checking if the entity has a component A **or** another component B.

#### Conclusion

This relatively simple elements let us easily compose entities by combining components together. In my experience, this leads to easier debugging and prototyping, because you can plug components in and out at will, or deactivate systems to check if some logic is improperly behaving.

## 2. Goals

The initial target of **Artecshpp** is to experiment with the ECS philosophy so that I can explore ways to make the inner behaviour flexible instead of sticking into one single strategy of handling the inner bits of an ECS. This also involves the way that memory management is handled, letting the user provide custom allocation.

I know this is just the opposite of what is recommended in order to finish a project, but I don't have any problem with artecshpp being unfinished. The presence of this document might give the project a *professional feel*, but that's definitely not the point. It is just a way to keep things making sense together, and why not, training myself in writing specs.
