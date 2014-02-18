BLOBS = GetArrayVariable("BLOBS")
hotCenterX = GetFloatVariable("/SmartDashboard/targetPixelXCenter")

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

    '' Display to debug console
    write vbCRLF & "X1: " & hotCenterX & " X2: " & verticalCenterX & " Side: " & side

    SetVariable "/SmartDashboard/Side", side
	end if
end if