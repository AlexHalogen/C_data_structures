# AlexHalogen's Data Structure Library

This is a collection of my implementation of some useful data structures in C.

Motivations for writing this library comes from my other project: [Minecraft Dungeon Finder](https://github.com/AlexHalogen/MC_DungeonFinder), where data structures like queues, vectors and hashmaps are required to write that Multi-threaded dungeon finder in C.

All these code are written for the purpose of **practicing**, so they should not be used in production.

Currently I have implemented:

- MT-safe FIFO queue: `queue.h`
- (Partly)MT-safe C++-like vector: `auto_array.h`
- (Partly)MT-safe hashmap: `hashmap.h`
- bitmap(Not the image format): `bitmap.h`
- doubly linked list: `utils/list.h`

Docs for hashmap, bitmap, and list are not completed because I'm still think about design details.

The library is licensed under Unlicense. Check `LICENSE` for details


## TODOs

- [ ] More comprehensive tests for auto_array and hashmap
- [ ] Restructure hashmap so that its code could be reused for hashset
- [ ] Implement hashset
- [ ] Implement a red-black tree
- [ ] Implement ordered_map and ordered_set
- [ ] ~~Better naming for functions, 'class'es and variables~~
