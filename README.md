# SoundProgramming

Visual Studio C++ OpenGL GameEngine with <a href="http://www.fmod.org/documentation/#content/generated/common/overview_lowlevel.html" target="_blank">FMOD Low Level API</a> made for a school assignment, 2016.

In this project are five open cubes (lets call them 'Rooms') with one open side and all the other sides slightly spread apart to allow sound to travel between them. Using FMOD Low Level API I added, ontop of the visual geometry, occlusion geometry which blocks and reflects sound. This can be heard a you move the camera around each of the Rooms, the walls don't completley block out sound but the amount of blocking can be adjusted from 0 - 100%.

On the rear face of the Room, a visual equaliser is drawn using a shader and the <a href="https://en.wikipedia.org/wiki/Fast_Fourier_transform" target="_blank">FFT Data</a> from each individual sound in each Room.

<a href="https://www.youtube.com/watch?v=IhPO4QzuYW0" target="_blank">Here is a video showing the project in action.</a>

<a href="https://www.youtube.com/watch?v=AvGXPVKxwwg" target="_blank">This is a shorter version with commentary.</a>
