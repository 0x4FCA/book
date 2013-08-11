# Objects

We've already seen several tools that OCaml provides for organizing programs,
particularly modules.  In addition, OCaml also supports object-oriented
programming.  There are objects, classes, and their associated types. In this
chapter, we'll introduce you to OCaml objects and subtyping.  In the next
chapter [xref](#classes), we'll introduce you to classes and inheritance.

<note>
<title>What is Object-Oriented Programming?</title>

Object-oriented programming (often shorted to OOP) is a programming style that
encapsulates computation and data within logical *objects*.  Each object
contains some data stored in *fields*, and has *method* functions that can be
invoked against the data within the object (also called "sending a message" to
the object).  The code definition behind an object is called a *class*, and
objects are constructed from a class definition by calling a constructor with
the data that the object will use to build itself.

There are five fundamental properties that differentiate OOP from other styles:

* _Abstraction_: the details of the implementation are hidden in the
  object, and the external interface is just the set of publicly-accessible
  methods.
* _Dynamic lookup_: when a message is sent to an object, the method to
  be executed is determined by the implementation of the object, not
  by some static property of the program.  In other words, different
  objects may react to the same message in different ways.
* _Subtyping_: if an object `a` has all the functionality of an object
  `b`, then we may use `a` in any context where `b` is expected.
* _Inheritance_: the definition of one kind of object can be reused
  to produce a new kind of object.  This new definition can override
  some behavior, but also share code with its parent.
* _Open recursion_: an object's methods can invoke another method in the same
  object using a special variable (often called `self` or `this`). When objects
  are created from classes, these calls use dynamic lookup, allowing a method
  defined in one class to invoke methods defined in another class that inherits
  from the first.

Almost every notable modern programming language has been influenced
by OOP, and you'll have run across these terms if you've ever used
C++, Java, C#, Ruby, Python or JavaScript.

</note>

## OCaml objects

If you already know about object oriented programming in a language like Java
or C++, the OCaml object system may come as a surprise.  Foremost is the
complete separation of objects, and their types, from the class system in
OCaml.  In a language like Java, a class name is also used as the type of
objects created by instantiating it, and the relationships between these object
types correspond to inheritance.  For example, if we implement a class `Deque`
in Java by inheriting from a class `Stack`, we would be allowed to pass a deque
anywhere a stack is expected.

OCaml is entirely different.  Classes are used to construct objects and support
inheritance, but classes are not types.  Instead, objects have _object types_,
and if you want to use objects, you aren't required to use classes at all.
Here's an example of a simple object.

```frag
((typ ocamltop)(name objects/stack.topscript)(part 1))
```

The object has an integer list value `v`, a method `pop` that returns the
head of `v`, and a method `push` that adds an integer to the head of
`v`.

The object type is enclosed in angle brackets `< ... >`, containing just the
types of the methods.  Fields, like `v`, are not part of the public
interface of an object.  All interaction with an object is through its methods.
The syntax for a method invocation uses the `#` character.

```frag
((typ ocamltop)(name objects/stack.topscript)(part 2))
```

Note that unlike functions, methods can have zero parameters, since the method
call is routed to a concrete object instance.  That's why the `push` and `pop`
methods do not have a `unit` argument, as the equivalent functional versions
would.

Objects can also be constructed by functions.  If we want to specify the
initial value of the object, we can define a function that takes the value and
returns an object.

```frag
((typ ocamltop)(name objects/stack.topscript)(part 3))
```

Note that the types of the function `stack` and the returned object now use the
polymorphic type `'a`.  When `stack` is invoked on a concrete value `[3; 2; 1]`, 
we get the same object type as before, with type `int` for the values on the 
stack.

## Object polymorphism ##

Like polymorphic variants, methods can be used without an explicit type declaration.

```frag
((typ ocamltop)(name objects/polymorphism.topscript)(part 1))
```

As you can see object types are inferred automatically from the methods that
are invoked on them.

The type system will complain if it sees incompatible uses of the same method:

```frag
((typ ocamltop)(name objects/polymorphism.topscript)(part 2))
```

The `..` in the inferred object types are ellipses, standing for other
unspecified methods that the object may have.  The type `< width : float; .. >`
specifies an object that must have at least a `width` method, and possibly some
others as well. Such object types are said to be _open_.

We can manually _close_ an object type using a type annotation:

```frag
((typ ocamltop)(name objects/polymorphism.topscript)(part 3))
```

<note>
<title>Elisions are polymorphic</title>

The `..` in an open object type is an elision, standing for "possibly more
methods."  It may not be apparent from the syntax, but an elided object type is
actually polymorphic.  For example, if we try to write a type definition, we
get an "unbound type variable" error.

```frag
((typ ocamltop)(name objects/polymorphism.topscript)(part 4))
```

This is because `..` is really a special kind of type variable called a _row
variable_.

This kind of typing scheme using row variables is called _row polymorphism_.
Row polymorphism is also used in polymorphic variant types, and there is a
close relationship between objects and polymorphic variants: objects are to
records what polymorphic variants are to ordinary variants.

</note>

An object of type `< pop : int option; .. >` can be any object with a method
`pop : int option`, it doesn't matter how it is implemented.  When the method
`#pop` is invoked, the actual method that is run is determined by the object.

```frag
((typ ocamltop)(name objects/stack.topscript)(part 4))
```

## Immutable objects ##

Many people consider object-oriented programming to be intrinsically
imperative, where an object is like a state machine.  Sending a
message to an object causes it to change state, possibly sending
messages to other objects.

Indeed, in many programs, this makes sense, but it is by no means required.
Let's define a function that creates immutable stack objects.

```frag
((typ ocamltop)(name objects/immutable.topscript)(part 1))
```

The key parts of this implementation are in the `pop` and `push` methods. The
expression `{< ... >}` produces a copy of the current object, with the same
type, and the specified fields updated.  In other words, the `push hd` method
produces a copy of the object, with `v` replaced by `hd :: v`.  The original
object is not modified.

```frag
((typ ocamltop)(name objects/immutable.topscript)(part 2))
```

There are some restriction on the use of the expression `{< ... >}`.
It can be used only within a method body, and only the values of
fields may be updated.  Method implementations are fixed at the time
the object is created, they cannot be changed dynamically.

## When to use objects

You might wonder when to use objects in OCaml, which has a multitude of
alternative mechanisms to express the similar concepts.  First-class modules
are more expressive (a module can include types, while classes and objects
cannot).  Modules, functors, and datatypes also offer a wide range of ways to
express program structure.  In fact, many seasoned OCaml programmers rarely use
classes and objects, if at all.

Objects have some advantages over records: they don't require type definitions
and their support for row polymorphism makes them more flexible. However, the
heavy syntax and additional runtime cost means that objects are rarely used in
place of records.

The real benefits of objects come from the class system. Classes support
inheritance and open recursion. Open recursion allows interdependent parts of
an object to be defined separately. This works because calls between the
methods of an object are determined when the object is instantiated, a form of
_late_ binding. This makes it possible (and necessary) for one method to
refer to other methods in the object without knowing statically how they will
be implemented.

In contrast, modules use early binding.  If you want to parameterize your
module code so that some part of it can be implemented later, you would write a
function or functor.  This is more explicit, but often more verbose than
overriding a method in a class.

In general, a rule of thumb is: use classes and objects in situations where
open recursion is a big win.  Two good examples are Xavier Leroy's
[Cryptokit](http://gallium.inria.fr/~xleroy/software.html#cryptokit), which
provides a variety of cryptographic primitives that can be combined in
building-block style, and the [Camlimages](http://cristal.inria.fr/camlimages/)
library which manipulates various graphical file formats.  Camlimages also
provides a module-based version of the same library, letting you choose between
functional and object-oriented styles depending on your problem domain.

We'll introduce you to classes, and examples using open recursion, in
[xref](#classes).

## Subtyping

Subtyping is a central concept in object-oriented programming.  It
governs when an object with one type _A_ can be used in an expression
that expects an object of another type _B_.  When this is true, we say
that _A_ is a _subtype_ of _B_.  Actually, more concretely, subtyping
restricts when the coercion operator `e :> t` can be applied.  This
coercion works only if the expression `e` has some type `s` and `s` is
a subtype of `t`.

### Width subtyping

To explore this, let's define some simple object types for geometric
shapes.  The generic type `shape` has a method to compute the area,
and `square` and `circle` are specific kinds of shape.

```frag
((typ ocaml)(name objects/subtyping.ml)(part 1))
```

A `square` has a method `area` just like a `shape`, and an additional
method `width`.  Still, we expect a `square` to be a `shape`, and it
is.  The coercion `:>` must be explicit.

```frag
((typ ocamltop)(name objects/subtyping.topscript)(part 1))
```

This form of object subtyping is called _width_ subtyping. Width
subtyping means that an object type _A_ is a subtype of _B_, if _A_
has all of the methods of _B_, and possibly more.  A `square` is a
subtype of `shape` because it implements all of the methods of `shape`
(the `area` method).

### Depth subtyping

We can also use _depth_ subtyping with objects. Depth subtyping, in its most
general form, says that an object type `< m: t1 >` is a subtype of `< m: t2 >`
iff `t1` is a subtype of `t2`.

For example, we can create two objects with a `shape` method:

```frag
((typ ocamltop)(name objects/subtyping.topscript)(part 2))
```

Both these objects have a `shape` method whose type is a subtype of the `shape`
type, so they can both be coerced into the object type `< shape : shape >`

```frag
((typ ocamltop)(name objects/subtyping.topscript)(part 3))
```

### Polymorphic variant subtyping

Subtyping can also be used to coerce a polymorphic variant into a larger
polymorphic variant type. A polymorphic variant type _A_ is a subtype of _B_,
if the tags of _A_ are a subset of the tags of _B_.

```frag
((typ ocamltop)(name objects/subtyping.topscript)(part 4))
```

### Variance

What about types built from object types? If a `square` is a `shape`, we expect
a `square list` to be a `shape list`. OCaml does indeed allow such coercions:

```frag
((typ ocamltop)(name objects/subtyping.topscript)(part 5))
```

Note that this relies on lists being immutable. It would not be safe to treat a
`square array` as a `shape array` because it would allow you to store
non-square shapes into what should be an array of squares. OCaml recognizes
this and does not allow the coercion.

```frag
((typ ocamltop)(name objects/subtyping.topscript)(part 6))
```

We say that `'a list` is _covariant_ (in `'a`), whilst `'a array` is
_invariant_. 

Subtyping function types requires a third class of variance. A function with
type `square -> string` cannot be used with type `shape -> string` because it
expects its argument to be a `square` and would not know what to do with a
`circle`. However, a function with type `shape -> string` _can_ safely be used
with type `square -> string`.

```frag
((typ ocamltop)(name objects/subtyping.topscript)(part 7))
```

We say that `'a -> string` is _contravariant_ in `'a`. In general, function types
are contravariant in their arguments and covariant in their results.

<note>
<title>Variance annotations</title>

OCaml works out the variance of a type using that type's definition. 

```frag
((typ ocamltop)(name objects/subtyping.topscript)(part 8))
```

However, if the definition is hidden by a signature then OCaml is forced to
assume that the type is invariant.

```frag
((typ ocamltop)(name objects/subtyping.topscript)(part 9))
```

We can fix this by adding _variance annotations_ to the type's parameters in the signature: `+`
for covariance or `-` for contravariance.

```frag
((typ ocamltop)(name objects/subtyping.topscript)(part 10))
```

</note>

For a more concrete example of variance, let's create some stacks containing
shapes by applying our `stack` function to some squares and some circles.


```frag
((typ ocaml)(name objects/subtyping.ml)(part 2))
```

If we wanted to write a function that took a list of such stacks and found the
total area of their shapes, we might try:

```frag
((typ ocamltop)(name objects/subtyping.topscript)(part 12))
```

However, when we try to apply this function to our objects we get an error:

```frag
((typ ocamltop)(name objects/subtyping.topscript)(part 13))
```

As you can see, `square stack` and `circle stack` are not subtypes of `shape
stack`.  The problem is with the `push` method.  For `shape stack`, the `push`
method takes an arbitrary `shape`.  So if we could coerce a `square stack` to a
`shape stack`, then it would be possible to push an arbitrary shape onto
`square stack`, which would be an error.

Another way of looking at this is that `< push: 'a -> unit; .. >` is
contravariant in `'a`, so `< push: square -> unit; pop: square option >` cannot be a
subtype of `< push: shape -> unit; pop: shape option >`.

Still, the `total_area` function should be fine, in principle.  It doesn't call
`push`, so it isn't making that error.  To make it work, we need to use a more
precise type that indicates we are not going to be using the set method.  We
define a type `readonly_stack` and confirm that we can coerce the list of
stacks to it.

```frag
((typ ocamltop)(name objects/subtyping.topscript)(part 14))
```

Aspects of this section may seem fairly complicated, but it should be pointed
out that this typing _works_, and in the end the type annotations are fairly
minor.  In most typed object-oriented languages, these coercions would simply
not be possible.  For example, in C++, a STL type `list<T>` is invariant in
`T`, it is simply not possible to use `list<square>` where `list<shape>` is
expected (at least safely).  The situation is similar in Java, although Java
has an escape hatch that allows the program to fall back to dynamic typing.
The situation in OCaml is much better; it works, it is statically checked, and
the annotations are pretty simple.

### Narrowing

Narrowing, also called _down casting_, is the ability to coerce an
object to one of its subtypes.  For example, if we have a list of
shapes `shape list`, we might know (for some reason) what the actual
type of each shape is.  Perhaps we know that all objects in the list
have type `square`.  In this case, _narrowing_ would allow the
re-casting of the object from type `shape` to type `square`.  Many
languages support narrowing through dynamic type checking.  For example,
in Java, a coercion `(Square) x` is allowed if the value `x` has type
`Square` or one of its subtypes; otherwise the coercion throws an
exception.

Narrowing is *not permitted* in OCaml.  Period.

Why?  There are two reasonable explanations, one based on a design principle,
and another technical (the technical reason is simple: it is hard to
implement).

The design argument is this: narrowing violates abstraction.  In fact,
with a structural typing system like in OCaml, narrowing would
essentially provide the ability to enumerate the methods in an object.
To check whether an object `obj` has some method `foo : int`, one
would attempt a coercion `(obj :> < foo : int >)`.

More commonly, narrowing leads to poor object-oriented style.
Consider the following Java code, which returns the name of a shape
object.

```frag
((typ java)(name objects/Shape.java))
```

Most programmers would consider this code to be "wrong."  Instead
of performing a case analysis on the type of object, it would be better
to define a method to return the name of the shape.  Instead of
calling `GetShapeName(s)`, we should call `s.Name()` instead.

However, the situation is not always so obvious.  The following code
checks whether an array of shapes looks like a "barbell," composed of
two `Circle` objects separated by a `Line`, where the circles have the
same radius.

```frag
((typ java)(name objects/IsBarbell.java))
```

In this case, it is much less clear how to augment the `Shape` class
to support this kind of pattern analysis.  It is also not obvious that
object-oriented programming is well-suited for this situation.
Pattern matching seems like a better fit.

```frag
((typ ocaml)(name objects/is_barbell.ml))
```
 
Regardless, there is a solution if you find yourself in this
situation, which is to augment the classes with variants.  You can
define a method `variant` that injects the actual object into a
variant type.
 
```frag
((typ ocaml)(name objects/narrowing.ml)(part 1))
```

This pattern works, but it has drawbacks.  In particular, the
recursive type definition should make it clear that this pattern is
essentially equivalent to using variants, and that objects do not
provide much value here.

### Subtyping vs. row polymorphism ###

There is considerable overlap between subtyping and row polymorphism. Both
mechanisms allow you to write functions that can be applied to objects of
different types. In these cases, row polymorphism is usually preferred over
subtyping because it does not require explicit coercions, and it preserves more
type information, allowing functions like the following:

```frag
((typ ocamltop)(name objects/row_polymorphism.topscript)(part 1))
```

The return type of this function is built from the open object type of its
argument, preserving any additional methods that it may have. 

```frag
((typ ocamltop)(name objects/row_polymorphism.topscript)(part 2))
```

Writing a similar function with a closed type and applying it using subtyping
does not preserve the methods of the argument: the returned object is only
known to have an `area` method.

```frag
((typ ocamltop)(name objects/row_polymorphism.topscript)(part 3))
```

However, there are some situations where we cannot use row polymorphism. In
particular, row polymorphism cannot be used to place different types of object
in the same container. For example, lists of heterogeneous elements cannot be
created using row polymorphism:

```frag
((typ ocamltop)(name objects/row_polymorphism.topscript)(part 4))
```

Similarly, we cannot use row polymorphism to store different types of object in
the same reference:

```frag
((typ ocamltop)(name objects/row_polymorphism.topscript)(part 5))
```

In both these cases we must use subtyping:

```frag
((typ ocamltop)(name objects/row_polymorphism.topscript)(part 5))
```

<note>
<title>Production note</title>

This chapter contains significant external contributions from Leo White.

</note>

