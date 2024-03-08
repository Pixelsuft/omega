import sys
import json

def hex_to_str(hexa):
    if hexa[0] == '#':
        hexa = hexa[1:]
    res = tuple(int(hexa[i:i+2], 16) for i in (0, 2, 4))
    # TODO: RGBA
    return f'({res[0]},{res[1]},{res[2]}),255'


inp = json.loads(open(sys.argv[1], 'r', encoding='utf-8').read())
out = open(sys.argv[2], 'w', encoding='utf-8')
out.write('PIXELSUFT_LDTK_TXT\n')
out.write(f'INFO,{inp["worldGridWidth"]},{inp["worldGridHeight"]},')
out.write(f'{hex_to_str(inp["bgColor"])}\n')

for lvl in inp['levels']:
    out.write(f'LEVEL,{lvl["uid"]},{lvl["identifier"]},{lvl["worldDepth"]},{lvl["worldX"]},{lvl["worldY"]},{lvl["pxWid"]},')
    out.write(f'{lvl["pxHei"]},{hex_to_str(lvl["bgColor"] or lvl["__bgColor"])}\n')
    for lay in lvl['layerInstances']:
        is_ent = lay['__type'] == 'Entities'
        out.write(f'LAYER,{lay["__identifier"]},{lay["levelId"]},{lay["layerDefUid"]},')
        out.write(f'{lay["__tilesetDefUid"] or -1},{int(is_ent)},')
        out.write(f'{lay["__cWid"]},{lay["__cHei"]},{lay["layerDefUid"]},{lay["__gridSize"]},{lay["__pxTotalOffsetX"]},')
        out.write(f'{lay["__pxTotalOffsetY"]},{lay["pxOffsetX"]},{lay["pxOffsetY"]},{int(lay["visible"])}\n')
        for ent in lay['entityInstances']:
            out.write('E,\n')
        for tile in (lay['autoLayerTiles'] or lay['gridTiles']):
            out.write('T,\n')

out.write('EOF!!!\n')
out.close()
