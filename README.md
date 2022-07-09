# Controle de temperatura com máquinas de estados em firmware ![Language grade: C](https://img.shields.io/badge/language-C-blue)

Autora: Lesly Montúfar

Controle de temperatura usando conceito de máquinas de estados em firmware.

## Índice 

* [Descrição](#descrição)
* [Desenvolvimento](#desenvolvimento)

## Descrição
Simular um controle de temperatura composto por um equipamento capaz
de resfriar ou aquecer um ambiente, operando como o diagrama a seguir.

![Diagrama](https://github.com/LeslyMontufar/ac_temp/blob/main/img/diagrama.png)

Para simplificação, considere as seguintes modificações nesse sistema:
* O sistema parte pré-programado para um set point de 25 graus. Nessa implementação, esse
valor não será alterado, ou seja, o propósito do equipamento é manter essa temperatura.
* A temperatura do ambiente será simulada usando dois jumpers, fazendo o papel de switches,
(+) e (-). Use o PA0 e PA1 para essas switches (não esqueça de habilitar o pull up interno). A
ideia é que o ambiente também esteja, inicialmente, a 25 graus.
* Ao pressionar a switch (+), a temperatura ambiente simulada deve subir um meio grau (use
uma variável para manter o valor da temperatura ambiente). Ao se pressionar a switch (-), a
temperatura ambiente simulada deve abaixar meio grau.
* Considere que a histerese é de dois graus. Ou seja, somente quando a temperatura ambiente
chegar a 27 graus a parte de resfriamento deve ser ligada. Da mesma forma, quando a
temperatura ambiente chegar a 23 graus, a parte de aquecimento deve ser ligada.
* Usando o que já foi construído em exercícios anteriores, crie um controle de piscada do led
dentro do systick. O comportamento do led deve ser o seguinte:
o Piscar a cada 500ms indica que a máquina está em idle, ou seja, nem o aquecimento
nem o resfriamento está ligado.
o Piscar a cada 100ms indica que o resfriamento está ligado.
o Piscar a cada um segundo indica que o aquecimento está ligado.
* O código da sua máquina de estado deve ficar no loop da sua aplicação.

Existem diversas estratégias de implementação, como visto em sala de aula, como switch cases, switch cases com funções e ainda utilizando ponteiro para funções. É interessante criar um objeto que represente a máquina, armazenando tudo que seja relacionado a ela (use uma estrutura pra isso) e crie enumerações para os estados. Não esqueça de definir uma função de inicialização ou reset e de tratar todos os estados.

Implemente a máquina de estados proposta nesse documento (escolha a versão que preferir).
Configure dois pinos disponíveis como GPIO de entrada e, usando jumpers, simule os botões de (+) e (-). Use o led da placa para indicar o estado da máquina, como descrito na proposição. Compile, debug e avalie o comportamento.

## Desenvolvimento
