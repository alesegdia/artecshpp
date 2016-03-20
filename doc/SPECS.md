# Artecshpp specs

<small>**This document is in alpha stage and will constantly evolve during development.**</small>

## Table of Contents

1. [What is an ECS?](#1-what-is-an-ecs)
2. [Goals](#2-goals)

---

## 1. What is an ECS?

A lot has been written related to Entity-Component systems focused on games, so I will barely sketch the concept. Basically, three basic core concepts are to be defined:

* **Component**. Set of closely related properties.
* **Entity**. Set of related components.
* **System**. Performs logic on one or several components of an entity.

This way, we avoid inheritance on behalf of composition, letting us easily compose entities by combining components together.

## 2. Goals

The initial target of **Artecshpp** is to experiment with the ECS philosophy so that I can explore ways to make the inner behaviour flexible instead of sticking into one single strategy of handling the inner bits of an ECS. This also involves the way that memory management is handled, letting the user provide custom allocation.

I know this is just the opposite of what is recommended in order to finish a project, but I don't have any problem with artecshpp being unfinished. The presence of this document might give the project a *professional feel*, but that's definitely not the point. It is just a way to keep things making sense together, and why not, training myself in writing specs.
