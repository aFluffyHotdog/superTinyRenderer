# superTinyRenderer

This repo is a little exercise in building a suuuuper basic C++ renderer from scratch that can read a `.obj` file then output a `.bmp` image

### Milestones:
- [x] draw a gradient on a `.bmp` file
- [x] follow the drawing triangles chapter of the tutorial
- [x] finish the wireframe of a demon example
- [x] finish barycentric coordinates shading
- [x] finish removing hidden faces chapter
- [x] refactor model code into an object

Missing Triangles Prolbem:
- Apparently the new scale method is deleting some faces which is not good.
- It might not be the scaling algo, I might've messed up the import step whoops
- Fixed the import step now, but it seems the scaling still causes some faces to not render properly? (tested by using the diablo model and copied import code from the tutorial)
- Based on the original code, it seems like we should convert everything into an integer before we pass it to the draw function
- BRO I SWAPPED X AND Y 

### Inspired By: (copied from)
- https://www.youtube.com/watch?v=yyJ-hdISgnw&t=2213s
- https://github.com/ssloy/tinyrenderer/wiki/Lesson-1:-Bresenham’s-Line-Drawing-Algorithm