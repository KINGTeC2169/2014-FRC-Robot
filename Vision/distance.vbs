list = GetArrayVariable("BFR_COORDINATES")
COG_X = GetVariable("COG_X")
COG_Y = GetVariable("COG_Y")
tracking = 0


if isArray(list) then
  if ubound(list) > 0 then
	tracking = 1
	targetPixelHeight = 0

	' This is the competition setting
	cameraFieldOfView = 47.5
	targetHeight = 4

	' Grab list of coordinates from blob_replace
	' Note the array is sorted by previous module in a specific order
	righty = list(1)
	rightyy = list(7)
	lefty = list(3)
	leftyy = list(5)

	' Based on these two side lines, get the center line height.
	' The center line is used since we want to aim to the center
	' of the target. This also removes any perspective distortion
	' where the right or left size may be a couple inches closer
	' or futher away from the camera

	targetPixelHeight = ((lefty - leftyy) + (righty - rightyy)) / 2

	SetVariable "/SmartDashboard/COG_X", COG_X
	SetVariable "/SmartDashboard/cLineTop", COG_Y + (targetPixelHeight / 2)
	SetVariable "/SmartDashboard/cLineBottom" , COG_Y - (targetPixelHeight / 2)
	end if
end if

write vbCRLF & "Distance (in): " & totalDistance
SetVariable "/SmartDashboard/Tracking", tracking
SetVariable "/SmartDashboard/Distance", CLng((totalDistance*100)/12)/100 