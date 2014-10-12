
Passo 0) Instalar o Visual Studio 2013 (nao pode ser o Express)

Passo 1) Sacar e instalar TortoiseSVN (que eh necessario para o visualSVN):
http://www.visualsvn.com/visualsvn/download/tortoisesvn/

Passo 2) Sacar e instalar VisualSVN:
http://www.visualsvn.com/visualsvn/download/

Passo 3) Abrir o Windows explorer. Na barra de endereços, colocar '%AppData%'. Entrar na pasta 'Subversion' e abrir com 
o notepad o ficheiro 'config'. Na secção [tunnels] colocar:

ssh = "C:/Program Files/TortoiseSVN/bin/TortoisePlink.exe"

Alterar as pastas para o que escolheram aquando da instalacao.

Caso nao haja directoria Subversion no %AppData%, abrir o windows explorer numa pasta temporaria qualquer. Fazer right
click, seleccionar TortoiseSVN->Import e colocar o url do repositorio (esta no passo 4).
Voltar ao inicio do passo 3.

Passo 4) Abrir o Visual Studio. Teem agora um menu chamado VisualSVN. Seleccionar VisualSVN->Get Solution from Subversion.

Colocar em Repository URL:
svn+ssh://ist1XXXXX@sigma.ist.utl.pt/afs/ist.utl.pt/users/2/9/ist149729/AVT/svn/

substituir ist1XXXXX pelo vosso username. É-vos pedido a password da vossa conta no sigma.

Digam-me se algo correr mal.
