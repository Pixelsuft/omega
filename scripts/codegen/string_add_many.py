to = 25

for i in range(2, to + 1):
    res = f'#define _OMG_STRING_ADD{i}(s'
    for j in range(1, i + 1):
        res += f', p{j}'
    res += ') do { '
    for j in range(1, i + 1):
        res += f'OMG_STRING_ADD(s, p{j}); '
    res += '} while(0)'
    print(res)

res = '#define _OMG_STRING_ADD_INTERNAL('
for j in range(1, to + 1):
    res += f'_{j}, '
res += 'NAME, ...) NAME\n#define OMG_STRING_ADD_MANY(string, ...) _OMG_STRING_ADD_INTERNAL(__VA_ARGS__'
for j in range(to, 0, -1):
    res += f', _OMG_STRING_ADD{j}'
res += ') (string, __VA_ARGS__)'
print(res)
