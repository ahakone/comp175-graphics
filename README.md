# Graphics

## Assignments
### Shapes
Tessellation of primitives, i.e., breaking up shapes into multiple triangles. 
The primitives are 
<ul>
	<li>Cube</li>
	<li>Cone</li>
	<li>Sphere</li>
	<li>Cylinder</li>
	<li>Torus</li>
	<li>Flower</li>
</ul>

### Camera
Camera object (perspective transformation).
Controls include
<ul>
	<li>Camera roll, pitch, yaw</li>
	<li>Camera view angle </li>
	<li>Camera position</li>
	<li>Camera look vector</li>
	<li>Near and far clipping plane</li>

</ul>

### Sceneview
Renders scenes defined in XML files. The sceneview program transforms the given vertices and normals from object space to world space and applies colors to the objects for rendering. 

### Intersect
Basic ray tracer that 
<ol>
	<li> Generates rays</li>
	<li>Finds the closest object along each ray</li>
	<li> Lights (illuminates) the scene </li>
</ol>

### Ray Tracing
Extension of Intersect to support 
<ul>
	<li>Reflections</li>
	<li>Texture mapping</li>
	<li>Shadows</li>
	<li>Point and directional lighting</li>
	<li> Antialiasing</li>
</ul>
