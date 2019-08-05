//Maya ASCII 2018 scene
//Name: cone.ma
//Last modified: Tue, Jun 18, 2019 01:29:35 PM
//Codeset: 1252
requires maya "2018";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2018";
fileInfo "version" "2018";
fileInfo "cutIdentifier" "201706261615-f9658c4cfc";
fileInfo "osv" "Microsoft Windows 8 Business Edition, 64-bit  (Build 9200)\n";
fileInfo "license" "student";
createNode transform -n "pCone1";
	rename -uid "6A064899-42DB-CB05-CCF4-8D95EF5A1896";
createNode mesh -n "pConeShape1" -p "pCone1";
	rename -uid "2247022D-4878-6104-ABDF-E69F34F73B01";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 0.49999997019767761 0.26620916160032071 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 43 ".uvst[0].uvsp[0:42]" -type "float2" 0.7377643 0.18895492
		 0.70225441 0.11926282 0.64694643 0.063954845 0.5772543 0.028444968 0.5 0.016209101
		 0.42274573 0.028444979 0.35305363 0.06395489 0.2977457 0.11926287 0.26223582 0.18895495
		 0.24999994 0.26620924 0.26223582 0.34346348 0.2977457 0.41315556 0.35305366 0.46846348
		 0.42274573 0.50397336 0.5 0.51620919 0.57725424 0.50397336 0.64694631 0.46846348
		 0.70225424 0.41315556 0.73776412 0.34346348 0.75 0.26620924 0.32087219 0.9349997
		 0.29167962 0.89106488 0.27286157 0.8417865 0.26533908 0.78957653 0.26948032 0.73699009
		 0.28508258 0.68660128 0.31138223 0.640876 0.34709209 0.60205233 0.3904644 0.57203043
		 0.43937635 0.55227965 0.49143401 0.54376668 0.5440895 0.5469082 0.59476572 0.56155032
		 0.64098233 0.58697665 0.68047738 0.62194246 0.71131784 0.66473657 0.73199421 0.71326447
		 0.7414946 0.76515102 0.73935401 0.81785673 0.72567713 0.86880195 0.70113355 0.91549331
		 0.5 0.26620924 0.50364894 0.78188741;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 22 ".vt[0:21]"  0.95105714 -0.5 -0.30901718 0.80901754 -0.5 -0.5877856
		 0.5877856 -0.5 -0.80901748 0.30901715 -0.5 -0.95105702 0 -0.5 -1.000000476837 -0.30901715 -0.5 -0.95105696
		 -0.58778548 -0.5 -0.8090173 -0.80901724 -0.5 -0.58778542 -0.95105678 -0.5 -0.30901706
		 -1.000000238419 -0.5 0 -0.95105678 -0.5 0.30901706 -0.80901718 -0.5 0.58778536 -0.58778536 -0.5 0.80901712
		 -0.30901706 -0.5 0.95105666 -2.9802322e-08 -0.5 1.000000119209 0.30901697 -0.5 0.9510566
		 0.58778524 -0.5 0.80901706 0.809017 -0.5 0.5877853 0.95105654 -0.5 0.309017 1 -0.5 0
		 0 -0.5 0 0 0.5 0;
	setAttr -s 60 ".ed[0:59]"  0 1 0 1 2 0 2 3 0 3 4 0 4 5 0 5 6 0 6 7 0
		 7 8 0 8 9 0 9 10 0 10 11 0 11 12 0 12 13 0 13 14 0 14 15 0 15 16 0 16 17 0 17 18 0
		 18 19 0 19 0 0 20 0 1 20 1 1 20 2 1 20 3 1 20 4 1 20 5 1 20 6 1 20 7 1 20 8 1 20 9 1
		 20 10 1 20 11 1 20 12 1 20 13 1 20 14 1 20 15 1 20 16 1 20 17 1 20 18 1 20 19 1 0 21 1
		 1 21 1 2 21 1 3 21 1 4 21 1 5 21 1 6 21 1 7 21 1 8 21 1 9 21 1 10 21 1 11 21 1 12 21 1
		 13 21 1 14 21 1 15 21 1 16 21 1 17 21 1 18 21 1 19 21 1;
	setAttr -s 40 -ch 120 ".fc[0:39]" -type "polyFaces" 
		f 3 -1 -21 21
		mu 0 3 1 0 41
		f 3 -2 -22 22
		mu 0 3 2 1 41
		f 3 -3 -23 23
		mu 0 3 3 2 41
		f 3 -4 -24 24
		mu 0 3 4 3 41
		f 3 -5 -25 25
		mu 0 3 5 4 41
		f 3 -6 -26 26
		mu 0 3 6 5 41
		f 3 -7 -27 27
		mu 0 3 7 6 41
		f 3 -8 -28 28
		mu 0 3 8 7 41
		f 3 -9 -29 29
		mu 0 3 9 8 41
		f 3 -10 -30 30
		mu 0 3 10 9 41
		f 3 -11 -31 31
		mu 0 3 11 10 41
		f 3 -12 -32 32
		mu 0 3 12 11 41
		f 3 -13 -33 33
		mu 0 3 13 12 41
		f 3 -14 -34 34
		mu 0 3 14 13 41
		f 3 -15 -35 35
		mu 0 3 15 14 41
		f 3 -16 -36 36
		mu 0 3 16 15 41
		f 3 -17 -37 37
		mu 0 3 17 16 41
		f 3 -18 -38 38
		mu 0 3 18 17 41
		f 3 -19 -39 39
		mu 0 3 19 18 41
		f 3 -20 -40 20
		mu 0 3 0 19 41
		f 3 0 41 -41
		mu 0 3 20 21 42
		f 3 1 42 -42
		mu 0 3 21 22 42
		f 3 2 43 -43
		mu 0 3 22 23 42
		f 3 3 44 -44
		mu 0 3 23 24 42
		f 3 4 45 -45
		mu 0 3 24 25 42
		f 3 5 46 -46
		mu 0 3 25 26 42
		f 3 6 47 -47
		mu 0 3 26 27 42
		f 3 7 48 -48
		mu 0 3 27 28 42
		f 3 8 49 -49
		mu 0 3 28 29 42
		f 3 9 50 -50
		mu 0 3 29 30 42
		f 3 10 51 -51
		mu 0 3 30 31 42
		f 3 11 52 -52
		mu 0 3 31 32 42
		f 3 12 53 -53
		mu 0 3 32 33 42
		f 3 13 54 -54
		mu 0 3 33 34 42
		f 3 14 55 -55
		mu 0 3 34 35 42
		f 3 15 56 -56
		mu 0 3 35 36 42
		f 3 16 57 -57
		mu 0 3 36 37 42
		f 3 17 58 -58
		mu 0 3 37 38 42
		f 3 18 59 -59
		mu 0 3 38 39 42
		f 3 19 40 -60
		mu 0 3 39 40 42;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
select -ne :time1;
	setAttr ".o" 1;
	setAttr ".unw" 1;
select -ne :hardwareRenderingGlobals;
	setAttr ".otfna" -type "stringArray" 22 "NURBS Curves" "NURBS Surfaces" "Polygons" "Subdiv Surface" "Particles" "Particle Instance" "Fluids" "Strokes" "Image Planes" "UI" "Lights" "Cameras" "Locators" "Joints" "IK Handles" "Deformers" "Motion Trails" "Components" "Hair Systems" "Follicles" "Misc. UI" "Ornaments"  ;
	setAttr ".otfva" -type "Int32Array" 22 0 1 1 1 1 1
		 1 1 1 0 0 0 0 0 0 0 0 0
		 0 0 0 0 ;
	setAttr ".fprt" yes;
select -ne :renderPartition;
	setAttr -s 2 ".st";
select -ne :renderGlobalsList1;
select -ne :defaultShaderList1;
	setAttr -s 4 ".s";
select -ne :postProcessList1;
	setAttr -s 2 ".p";
select -ne :defaultRenderingList1;
select -ne :initialShadingGroup;
	setAttr -s 5 ".dsm";
	setAttr ".ro" yes;
select -ne :initialParticleSE;
	setAttr ".ro" yes;
select -ne :defaultResolution;
	setAttr ".pa" 1;
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
select -ne :ikSystem;
	setAttr -s 4 ".sol";
connectAttr "pConeShape1.iog" ":initialShadingGroup.dsm" -na;
// End of cone.ma
