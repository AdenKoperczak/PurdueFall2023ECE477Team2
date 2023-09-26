PULSONIX_LIBRARY_ASCII "SamacSys ECAD Model"
//1273225/1223953/2.50/2/3/Inductor

(asciiHeader
	(fileUnits MM)
)
(library Library_1
	(padStyleDef "r510_365"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 3.65) (shapeHeight 5.1))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0) (shapeHeight 0))
	)
	(textStyleDef "Normal"
		(font
			(fontType Stroke)
			(fontFace "Helvetica")
			(fontHeight 1.27)
			(strokeWidth 0.127)
		)
	)
	(patternDef "INDPM120120X1020N" (originalName "INDPM120120X1020N")
		(multiLayer
			(pad (padNum 1) (padStyleRef r510_365) (pt -4.85, 0) (rotation 0))
			(pad (padNum 2) (padStyleRef r510_365) (pt 4.85, 0) (rotation 0))
		)
		(layerContents (layerNumRef 18)
			(attr "RefDes" "RefDes" (pt 0, 0) (textStyleRef "Normal") (isVisible True))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt -6.925 6.4) (pt 6.925 6.4) (width 0.05))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt 6.925 6.4) (pt 6.925 -6.4) (width 0.05))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt 6.925 -6.4) (pt -6.925 -6.4) (width 0.05))
		)
		(layerContents (layerNumRef Courtyard_Top)
			(line (pt -6.925 -6.4) (pt -6.925 6.4) (width 0.05))
		)
		(layerContents (layerNumRef 28)
			(line (pt -6 6) (pt 6 6) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt 6 6) (pt 6 -6) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt 6 -6) (pt -6 -6) (width 0.025))
		)
		(layerContents (layerNumRef 28)
			(line (pt -6 -6) (pt -6 6) (width 0.025))
		)
		(layerContents (layerNumRef 18)
			(line (pt 6 6) (pt -6 6) (width 0.2))
		)
		(layerContents (layerNumRef 18)
			(line (pt -6 -6) (pt 6 -6) (width 0.2))
		)
	)
	(symbolDef "MSS1210-103MEB" (originalName "MSS1210-103MEB")

		(pin (pinNum 1) (pt 0 mils 0 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName false)) (pinName (text (pt 0 mils -35 mils) (rotation 0]) (justify "UpperLeft") (textStyleRef "Normal"))
		))
		(pin (pinNum 2) (pt 800 mils 0 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName false)) (pinName (text (pt 800 mils -35 mils) (rotation 0]) (justify "UpperRight") (textStyleRef "Normal"))
		))
		(arc (pt 250 mils -2 mils) (radius 50 mils) (startAngle 177.7) (sweepAngle -175.4) (width 6 mils))
		(arc (pt 350 mils -2 mils) (radius 50 mils) (startAngle 177.7) (sweepAngle -175.4) (width 6 mils))
		(arc (pt 450 mils -2 mils) (radius 50 mils) (startAngle 177.7) (sweepAngle -175.4) (width 6 mils))
		(arc (pt 550 mils -2 mils) (radius 50 mils) (startAngle 177.7) (sweepAngle -175.4) (width 6 mils))
		(attr "RefDes" "RefDes" (pt 650 mils 250 mils) (justify Left) (isVisible True) (textStyleRef "Normal"))
		(attr "Type" "Type" (pt 650 mils 150 mils) (justify Left) (isVisible True) (textStyleRef "Normal"))

	)
	(compDef "MSS1210-103MEB" (originalName "MSS1210-103MEB") (compHeader (numPins 2) (numParts 1) (refDesPrefix L)
		)
		(compPin "1" (pinName "1") (partNum 1) (symPinNum 1) (gateEq 0) (pinEq 0) (pinType Unknown))
		(compPin "2" (pinName "2") (partNum 1) (symPinNum 2) (gateEq 0) (pinEq 0) (pinType Unknown))
		(attachedSymbol (partNum 1) (altType Normal) (symbolName "MSS1210-103MEB"))
		(attachedPattern (patternNum 1) (patternName "INDPM120120X1020N")
			(numPads 2)
			(padPinMap
				(padNum 1) (compPinRef "1")
				(padNum 2) (compPinRef "2")
			)
		)
		(attr "Mouser Part Number" "994-MSS1210-103MEB")
		(attr "Mouser Price/Stock" "https://www.mouser.co.uk/ProductDetail/Coilcraft/MSS1210-103MEB?qs=VJjuEbE9QBNwjoERzwkOMg%3D%3D")
		(attr "Manufacturer_Name" "COILCRAFT")
		(attr "Manufacturer_Part_Number" "MSS1210-103MEB")
		(attr "Description" "Fixed Inductors MSS1210 AEC-Q200 10 uH 20 % 10 A")
		(attr "<Hyperlink>" "https://g.componentsearchengine.com/Datasheets/34/MSS1210-103MEB.pdf")
		(attr "<Component Height>" "10.2")
		(attr "<STEP Filename>" "MSS1210-103MEB.stp")
		(attr "<STEP Offsets>" "X=0;Y=0;Z=0")
		(attr "<STEP Rotation>" "X=0;Y=0;Z=0")
	)

)
