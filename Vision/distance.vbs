list = GetArrayVariable("BFR_COORDINATES")
tracking = 0


if isArray(list) then
  if ubound(list) > 0 then
  tracking = 1
	targetPixelHeight = 0

  imageHeight = GetVariable("IMAGE_HEIGHT")

	' This is the competition setting
  cameraFieldOfView = 47.5
	targetHeight = 4
	
  ' Grab list of coordinates from blob_replace
	' Note the array is sorted by previous module in a specific order
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
    targetPixelXCenter = ((leftx + leftxx + rightx + rightxx)) / 4
    SetVariable "/SmartDashboard/targetPixelXCenter", targetPixelXCenter
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
	end if
end if

write vbCRLF & "Distance (in): " & totalDistance
SetVariable "/SmartDashboard/Tracking", tracking
SetVariable "/SmartDashboard/Distance", CLng((totalDistance*100)/12)/100 