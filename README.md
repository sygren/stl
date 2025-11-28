# Todo cores:

- Array (Fixed size) Done
- LinkedList (double and simple) Done
- HashMap (simple chaining, later open adressing)
- HashSet
- make magic enum performant and work on msvc

# Todo vulkan:

## rewrite clean abstractions:

- buffer (no longer force into type whever device or host)
- graphic pipeline (no longer force render pass or something like that into it?)
- image
- sampler 
- texture 

## integrate ECS ? (probably for later i need fast prototyping?)

How can I integrate ECS in a way where the renderer automatically renders
stuff that are being spawned into the world? 

For now im forced to make stuff appears in the renderer code which kinda sucks..

