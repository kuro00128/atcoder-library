import sys, re, html, os

src = sys.stdin.read()
m = re.search(r'<span class="lang-ja">(.*?)</span>\s*<span class="lang-en">', src, re.S)
body = m.group(1) if m else src

def grab(tag):
    return re.findall(r'<h3>' + tag + r'\s*(\d+)\s*</h3>\s*<pre[^>]*>(.*?)</pre>', body, re.S)

ins, outs = grab('入力例'), grab('出力例')
os.makedirs('test', exist_ok=True)
for k, s in ins:
    open(f'test/sample-{k}.in', 'w').write(html.unescape(s).lstrip('\r\n'))
for k, s in outs:
    open(f'test/sample-{k}.out', 'w').write(html.unescape(s).lstrip('\r\n'))
print(f'{len(ins)} samples saved')