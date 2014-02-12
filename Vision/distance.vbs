list = GetArrayVariable("BFR_COORDINATES")
if isArray(list) then
  if ubound(list) > 0 then

	targetPixelHeight = 0
    targetSamples = 0
    imageHeight = GetVariable("IMAGE_HEIGHT")

	''This is the test setting
  	cameraFieldOfView = 47.5
	targetHeight = 6.75
	
	'' calibrated for an Axis camera
  	' cameraFieldOfView = 47.5
	' targetHeight = 37

    ' grab list of coordinates from blob_replace
	' note the array is sorted by previous module in a specific order
    righty = list(1)
    rightyy = list(7)
    lefty = list(3)
    leftyy = list(5)

    rightx = list(0)
    rightxx = list(6)
    leftx = list(2)
    leftxx = list(4)

	' Based on these two side lines, get the center line height.
	' The center line is used since we want to aim to the center
  	' of the target. This also removes any perspective distortion
	' where the right or left size may be a couple inches closer
	' or futher away from the camera
  	targetPixelHeight = ((lefty - leftyy) + (righty - rightyy)) / 2

  	' Calculate the x-coordinate of the center line by averaging
  	' the x-component of all four coordinates
  	SetVariable "/SmartDashboard/targetPixelXCenter", ((leftx + leftxx + rightx + rightxx)) / 4
  	targetPixelYCenter = ((lefty + leftyy + righty + rightyy)) / 4

  	' Draw the center line
  	SetVariable "/SmartDashboard/cLineTop", targetPixelYCenter + (targetPixelHeight / 2)
  	SetVariable "/SmartDashboard/cLineBottom" , targetPixelYCenter - (targetPixelHeight / 2)

    ' determine distance in inches
    if targetPixelHeight = 0 then
        totalDistance = -1
    else
        totalDistance = (((targetHeight*imageHeight)/targetPixelHeight)/2)/tan(((cameraFieldOfView*3.14159)/180.0)/2.0)
    end if
	write vbCRLF & "Distance (in): " & totalDistance
	SetVariable "/SmartDashboard/Distance", CLng((totalDistance*100)/12)/100


    '''' we can use a known distance to determine FOV if we don't know it
	''''  measuredDistance = 5.75*12.0
 	''''  write "Calculated FOV " & (((atan((((targetHeight*imageHeight)/targetPixelHeight)/2)/measuredDistance)*2.0)*180.0)/3.14159) & vbCRLF

	end if
end if
