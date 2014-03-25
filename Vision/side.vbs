BLOBS = GetArrayVariable("BLOBS")
hotCenterX = GetFloatVariable("/SmartDashboard/targetPixelXCenter")
tracking = GetFloatVariable("/SmartDashboard/Tracking")
side = -1

if isArray(BLOBS) then
  if ubound(BLOBS) > 0 then

  	'' Grab the x-center of the vertical strip 
    verticalCenterX = BLOBS(0)
    SetVariable "/SmartDashboard/verticalCenterX", verticalCenterX

    '' Determine which side is the hot goal
    if (hotCenterX < verticalCenterX) then
    	'' Left
    	side = 0
    elseif (hotCenterX > verticalCenterX) then
    	'' Right
      side = 1
    else
    	'' Something is wrong.
      side = -1
    end if

	end if
end if

if (tracking = 0) then 
	side = -1
end if

'' Display to debug console
write vbCRLF & "X1: " & hotCenterX & " X2: " & verticalCenterX & " Side: " & side
SetVariable "/SmartDashboard/Side", side
