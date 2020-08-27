import json
import maya.cmds as cmds
import math
import pymel

jsonData = {}  
jsonData['GameObjects'] = []  

selected_obj = cmds.ls(sl=True, type='transform')
for x in range(0, len(selected_obj)):
	node = pymel.core.ls(sl=1)[x]
	translation = cmds.xform(selected_obj[x], query=True, translation=True, worldSpace=True)
	orientation = node.getRotation(quaternion=True)
	scale1 = cmds.xform(selected_obj[x], query=True, scale=True, worldSpace=True)
    
	if node.hasAttr('notes'):
		notelist = node.notes.get().split('\n')
		gameObjectType = notelist[0]
		gameObjectName = notelist[1]

	if gameObjectType == "LevelGateObject":
		jsonData['GameObjects'].append(
		{
			'type': gameObjectType,
			'model': gameObjectName,
			'position': [translation[0], translation[1], translation[2]],
			'orientation': [orientation[0], orientation[1], orientation[2], orientation[3]],
			'scale': [scale1[0], scale1[1], scale1[2]],
			'nextLevel': notelist[2]
		})
	elif gameObjectType != "":
		jsonData['GameObjects'].append(
		{
			'type': gameObjectType,
			'model': gameObjectName,
			'position': [translation[0], translation[1], translation[2]],
			'orientation': [orientation[0], orientation[1], orientation[2], orientation[3]],
			'scale': [scale1[0], scale1[1], scale1[2]]
		})

basicFilter = "*.json"
unicodePath = cmds.fileDialog2(caption="Select save location", fileFilter=basicFilter, dialogStyle=2, fileMode=0, returnFilter=1)

if unicodePath != None and unicodePath != "":
	cleanPath = str(unicodePath[0])

with open(unicodePath[0], 'w') as outfile:
	json.dump(jsonData, outfile, sort_keys=False, indent=2)