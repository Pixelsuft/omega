import sys
import json
from PIL import ImageColor

def hex_to_str(hexa):
    if not hexa[0] == '#':
        hexa = '#' + hexa
    res = ImageColor.getcolor(hexa, 'RGBA')
    return f'({res[0]},{res[1]},{res[2]},{res[3]})'


inp = json.loads(open(sys.argv[1], 'r', encoding='utf-8').read())
out = open(sys.argv[2], 'w', encoding='utf-8')
out.write('PIXELSUFT_LDTK_TXT\n')
out.write(f'INFO,{inp["worldGridWidth"]},{inp["worldGridHeight"]},')
out.write(f'{hex_to_str(inp["bgColor"])}\n')

# TODO: tags support

for ent in inp['defs']['entities']:
    out.write(f'F,{ent["uid"]},"{ent["identifier"]}",{ent["width"]},{ent["height"]}\n')

for ts in inp['defs']['tilesets']:
    out.write(f'S,{ts["uid"]},"{ts["identifier"]}","{ts["relPath"]}",{ts["pxWid"]},')
    out.write(f'{ts["pxHei"]},{ts["tileGridSize"]},{ts["spacing"]},{ts["padding"]}\n')

for lvl in inp['levels']:
    out.write(f'LEVEL,{lvl["uid"]},"{lvl["identifier"]}",{lvl["worldDepth"]},{lvl["worldX"]},{lvl["worldY"]},{lvl["pxWid"]},')
    out.write(f'{lvl["pxHei"]},{hex_to_str(lvl["bgColor"] or lvl["__bgColor"])}\n')
    for lay in lvl['layerInstances'][::-1]:
        is_ent = lay['__type'] == 'Entities'
        out.write(f'LAYER,"{lay["__identifier"]}",{lay["levelId"]},{lay["layerDefUid"]},')
        out.write(f'{lay["overrideTilesetUid"] or lay["__tilesetDefUid"] or -1},{int(is_ent)},')
        out.write(f'{lay["__cWid"]},{lay["__cHei"]},{lay["__gridSize"]},{lay["__pxTotalOffsetX"]},')
        out.write(f'{lay["__pxTotalOffsetY"]},{lay["pxOffsetX"]},{lay["pxOffsetY"]},{int(lay["visible"])}\n')
        for ent in lay['entityInstances']:
            out.write(f'E,{ent["defUid"]},{ent["px"][0]},{ent["px"][1]},{ent["__grid"][0]},{ent["__grid"][1]}')
            out.write(f',{ent["width"]},{ent["height"]}\n')
        for tile in (lay['autoLayerTiles'] or lay['gridTiles']):
            out.write(f'T,{tile["src"][0]},{tile["src"][1]},{tile["px"][0]},{tile["px"][1]}')
            out.write(f',{tile["f"]},{tile["t"]},{tile["a"]}\n')

out.write('EOF!!!\n')
out.close()
