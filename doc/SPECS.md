# Artecshpp specs

<small>**This document is in alpha stage and will constantly evolve during development.**</small>

## Table of Contents

1. [What is an ECS?](#1-what-is-an-ecs)
2. [Goals](#2-goals)
3. [System overview](#3-system-overview)
  1. [Entity operations](#3-1-entity-operations)
  2. [Aspects](#3-2-aspects)
  3. [Entity filtering](#3-3-entity-filtering)


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

The initial target of **Artecshpp** is to experiment with the ECS philosophy so that I can explore ways to make the inner behaviour flexible instead of sticking into one single strategy of handling the inner bits of an ECS.

I know this is just the opposite of what is recommended in order to finish a project, but I don't have any problem with artecshpp being unfinished. The presence of this document might give the project a *professional feel*, but that's definitely not the point. It is just a way to keep things making sense together, and why not, training myself in writing specs.

## 3. System overview

In this section, I aim to describe the system's core concepts in detail. As mentioned previously, the design seeks flexibility at internal ECS functionality.

### 3.1. Entity operations

The system targets the following operations on entities at any time in the game life:

1. Creation.
2. Component addition.
3. Component removal.
4. Destruction.

### 3.2. Aspects

An aspect is basically a way to tell if a specific entity should be processed by a specific system, depending on which components the entity has. This way, a system will process an entity if and only if this entity is accepted by the system's owned aspect.

An aspect has the following conditioners to accept an entity:

* The entity **must have** certain components.
* The entity **must not have** certain components.
* The entity **can have** certain components.

In practice, the most common conditioner is the first one, but it's not hard to implement the others, and they can come in handy at some point.

### 3.3. Entity filtering strategies

We already have a way for a system to tell if it wants a specific entity via aspects. Now, we need a way to tell a system what set of entities the system must act on. Basically, two methods arise, and each one has its pros and cons.

#### Storage filtering

This method focuses on maintaining a list of entities that a specific system must act on. For this task, the object with this responsibility (storage from now on) needs to be notified on all possible entity operations (see [entity operations](#3-1-entity-operations)) so that the storage can add or remove entities depending on its changes and lifetime.

The pro of this method is that while entities does not change its state (add/remove component, creation, destruction), we don't need to perform any extra operations. The drawback is that we must keep track of entity changes and this add some extra complexity from a coding point of view.

#### Deterministic filtering

This method consists in iterating the whole list of entities at each system step. The direct advantage of this method is that we do not need to keep track of entities operations, but the drawback is the overhead of having to iterate over the whole list of living entities at each step.
