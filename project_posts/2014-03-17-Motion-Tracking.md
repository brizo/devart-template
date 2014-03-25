Tracking Motion:

We decided to use OpenNI and the Kinect to track the motion of the viewer.  We evaluated between that and ofXKinect which uses OpenCV for blob detetcion, but we wanted 
to detect hands and limbs for when there are "hits" in the movement, or change in direction, and for triggering sound.  

We first got ofxOpenNI examples to work and set to pull point cloud data.  We liked the idea that we could pull a point cloud of the viewer into the art piece and make them a part of it.  
So our goal was to incorporate it somehow into the whole piece.  

This was one thing that we came up with:
![Sketch Image](../project_images/particleconcept.jpg?raw=true "Sketch Image")


Next was to somehow incorporate movement into the whole piece, and generating this point cloud through movement.  Our idea was to use the hands and movement of the viewer to generate particles that would fly through space and create the particle image of the viewer that we capture at the beginning.  
