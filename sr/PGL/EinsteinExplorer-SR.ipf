
:userdoc.
:h1 res=100.Einstein Explorer - SR
:i1.general help
:p.
Einstein Explorer - SR, simulates the visual effects that occur when travelling at
relativistic speeds. At these speeds moving objects appear distorted due to light
having a finite invariant speed. There are two contributing factors to this visual
effect:
:ol.
:li.:hp2.The Lorentz Transformation:ehp2.
:p.
This is the transformation between different inertial frames. It is a linear
transformation in 4-dimensional space-time. In 3-dimensional space it causes a
parameterised (speed dependent) scaling and translation. The result is that
moving objects appear contracted along their direction of travel.
:li.:hp2.The Eye:ehp2.
:p.
The second effect is a consequence of the eye occupying a point in space.
What we see at any instance in time is light rays that have arrived at our eye
at the same time. The important thing to realise is that this does not imply
that the rays left at the same time. Light has to leave earlier from points that
are further away from the line of sight. For a moving object, different times
correspond to different positions. The final image we see is thus a composite
of images of the object in different positions.
:eol.
:p.
Adding perspective completes the effect.
:p.
You can press F1 for help at any time during the program.

:h1 res=200.Control Menu
:i1.control
:p.
The control menu provides options for transforming the current scene.
:ul.
:li.:link reftype=hd res=210.Translation:elink.
:li.:link reftype=hd res=220.Rotation:elink.
:li.:link reftype=hd res=230.Time:elink.
:li.:link reftype=hd res=240.Speed:elink.
:eul.

:h2 res=210.Translation Control
:i1.translation
:p.
Using the translation control you can adjust the position of the viewpoint.
All movement is done relative to a grand co-ordinate system.

:h3 res=211.X Increment Button
:p.
Click to move the viewpoint along the x-axis in the positive direction.

:h3 res=212.X Decrement Button
:p.
Click to move the viewpoint along the x-axis in the negative direction.

:h3 res=213.Y Increment Button
:p.
Click to move the viewpoint along the y-axis in the positive direction.

:h3 res=214.Y Decrement Button
:p.
Click to move the viewpoint along the y-axis in the negative direction.

:h3 res=215.Z Increment Button
:p.
Click to move the viewpoint along the z-axis in the positive direction.

:h3 res=216.Z Decrement Button
:p.
Click to move the viewpoint along the z-axis in the negative direction.

:h3 res=217.Scale Check Box
:p.
Tick to scale all translation movements by a factor of 10.
Useful for rapid navigation of the scene.

:h2 res=220.Rotation Control
:i1.rotation
:p.
Using the rotation control you can adjust the angle of the viewpoint.
The x,y,z slider bars change the component values of the axis vector about
which the rotation is applied.
All movement is done relative to the local co-ordinate system.

:h3 res=221.Angle Circular Slider Bar
:p.
Select the angle of rotation.

:h3 res=222.X Slider Bar
:p.
Drag the bar to set the x-component of the rotation axis vector.

:h3 res=223.Y Slider Bar
:p.
Drag the bar to set the y-component of the rotation axis vector.

:h3 res=224.Z Slider Bar
:p.
Drag the bar to set the z-component of the rotation axis vector.

:h2 res=230.Time Control
:i1.time
:p.
The time control allows you to step the current time forwards or backwards.

:h3 res=231.Time Increment Button
:p.
Click to increase the current time.

:h3 res=232.Time Decrement Button
:p.
Click to decrease the current time.

:h3 res=233.Scale Check Box
:p.
Tick to scale all temporal motion by a factor of 10.
Useful for rapid navigation of the scene.

:h2 res=240.Speed Control
:i1.speed
:p.
The speed control allows you to select the speed of the travelling
objects in the scene.

:h3 res=241.Speed Slider Bar
:p.
Drag the bar to set the speed.
The speed increases towards the right (zero at the left end).

:h1 res=300.Options Menu
:i1.options
:p.
There are several options available for changing the scene.
:ul.
:li.:link reftype=hd res=310.Frame:elink.
:li.:link reftype=hd res=320.Object:elink.
:li.:link reftype=hd res=330.Doppler:elink.
:li.:link reftype=hd res=340.Freeze:elink.
:li.:link reftype=hd res=350.Loop:elink.
:eul.

:h2 res=310.Frame Sub-Menu
:i1.frame
:p.
The frame sub-menu allows you to swap the viewpoint between the rest frame
of the world (default) and the rest frame of the moving object.

:h2 res=320.Object Sub-Menu
:i1.object
:p.
The object sub-menu allows you to select the shape of the moving object.

:h2 res=330.Doppler
:i1.doppler
:p.
Toggles the colour Doppler shift.
When enabled, the colour is set to black if the frequency is shifted out
of the visible range.

:h2 res=340.Freeze
:i1.freeze
:i1.animation
:p.
Toggles the time freeze. Unfreezing time causes the scene to become animated.

:h2 res=350.Loop
:i1.loop
:p.
Loops time.

:h1 res=400.Graphics Menu
:i1.graphics
:p.
There are several options available for improving the graphics.
:ul.
:li.:link reftype=hd res=410.Detail:elink.
:li.:link reftype=hd res=420.Fog:elink.
:li.:link reftype=hd res=430.Texture:elink.
:li.:link reftype=hd res=440.Wireframe:elink.
:eul.

:h2 res=410.Detail Control
:i1.detail
:p.
Controls subdivision.
This alters the accuracy/performance trade-off.

:h3 res=411.Detail Entry Field
:p.
At high speeds, straight lines become curves.
This value determines how accurately the curves are drawn.
The higher the value, the more accurate the curves and the slower the performance.
It must be between 0 and 1.
Effective range is between 0.99 and 1.

:h2 res=420.Fog
:i1.fog
:p.
Fog provides a depth-cueing effect, generating a more realistic image.
This decreases performance.

:h2 res=430.Texture
:i1.texture
:p.
Toggles texture mapping, creating a more realistic image.
This decreases performance.

:h2 res=440.Wireframe
:i1.wireframe
:p.
Toggles the wireframe view.
This increases performance.

:euserdoc.


