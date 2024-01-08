import os


cwd = os.path.dirname(__file__)
if cwd:
    cwd = os.path.join(cwd, '..', '..')
else:
    cwd = os.getcwd()

res = ''
for i in os.listdir(os.path.join(cwd, 'src')):
    res += open(os.path.join(cwd, 'src', i), 'r', encoding='utf-8').read() + '\n'
for i in os.listdir(os.path.join(cwd, 'include', 'omega')):
    res += open(os.path.join(cwd, 'include', 'omega', i), 'r', encoding='utf-8').read() + '\n'

spl = res.split('\n')
while '' in spl:
    spl.remove('')

symbols = res.replace('\n', '').replace(' ', '').replace('   ', '')

print('Symbols Count:', len(symbols))
print('Total Lines:', len(spl))
