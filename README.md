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
  * Piscar a cada 500ms indica que a máquina está em idle, ou seja, nem o aquecimento
nem o resfriamento está ligado.
  * Piscar a cada 100ms indica que o resfriamento está ligado.
  * Piscar a cada um segundo indica que o aquecimento está ligado.
* O código da sua máquina de estado deve ficar no loop da sua aplicação.

Existem diversas estratégias de implementação, como visto em sala de aula, como switch cases, switch cases com funções e ainda utilizando ponteiro para funções. É interessante criar um objeto que represente a máquina, armazenando tudo que seja relacionado a ela (use uma estrutura pra isso) e crie enumerações para os estados. Não esqueça de definir uma função de inicialização ou reset e de tratar todos os estados.

Implemente a máquina de estados proposta nesse documento (escolha a versão que preferir).
Configure dois pinos disponíveis como GPIO de entrada e, usando jumpers, simule os botões de (+) e (-). Use o led da placa para indicar o estado da máquina, como descrito na proposição. Compile, debug e avalie o comportamento.

## Desenvolvimento
1. Para o led piscar por interrupção, usa-se o temporizador do sistema SysTick:

```
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */
  app_tick_1ms();
  /* USER CODE END SysTick_IRQn 1 */
}
```

2. Para adicionar o botões cujo acionamento será uma interrupção para mudar delay do led PC13 e acender o led de HEAT ou COOL, conforme a situação, utilizou-se as funções de callback do barramento de interrupção:
```
void EXTI0_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI0_IRQn 0 */
#if 0
  /* USER CODE END EXTI0_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(SWT_DOWN_Pin);
  /* USER CODE BEGIN EXTI0_IRQn 1 */
#else
  __HAL_GPIO_EXTI_CLEAR_IT(SWT_DOWN_Pin);
//  ac_stw_down_callback();
  ac_stw_down_callback();
#endif
  /* USER CODE END EXTI0_IRQn 1 */
}

/**
  * @brief This function handles EXTI line1 interrupt.
  */
void EXTI1_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI1_IRQn 0 */
#if 0
  /* USER CODE END EXTI1_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(SWT_UP_Pin);
  /* USER CODE BEGIN EXTI1_IRQn 1 */
#else
  __HAL_GPIO_EXTI_CLEAR_IT(SWT_UP_Pin);
  ac_stw_up_callback();
#endif
  /* USER CODE END EXTI1_IRQn 1 */
}
```

```
void ac_stw_down_callback(void){

	static uint32_t debouncing_time_ms = 0;
	if((hw_tick_ms_get() - debouncing_time_ms) >= APP_DEBOUNCING_TIME_MS){
		room_temp -= 0.5;
		debouncing_time_ms = hw_tick_ms_get();
	}
}

void ac_stw_up_callback(void){

	static uint32_t debouncing_time_ms = 0;
	if((hw_tick_ms_get() - debouncing_time_ms) >= APP_DEBOUNCING_TIME_MS){
		room_temp += 0.5;
		debouncing_time_ms = hw_tick_ms_get();
	}
}
```
3. O acendimento dos led de HEAT e COOL ocorre nas seguintes funções chamadas na máquina de estados:
```
void ac_cool_set(bool state){
	hw_cool_state_set(state);
}

void ac_heat_set(bool state){
	hw_heat_state_set(state);
}
```

4. Inicialização das variáveis:

```
#define APP_DEBOUNCING_TIME_MS 	100		// com jumper o tempo de debouncing precisa ser maior
#define DELAY_IDLE 				500
#define DELAY_COOLING 			100
#define DELAY_HEATING 			1000

volatile float room_temp = 25.0;
volatile uint32_t delay = DELAY_IDLE;
bool app_started = false;

typedef enum {
	AC_INIT = 0,
	AC_IDLE,
	AC_HEAT,
	AC_COOL
} ac_states_t;

struct ac_sm_tt{
	ac_states_t state;
	float set_point_c;
	float histerese_c;
};

ac_sm_t ac_sm = {0};
```

5. Tem-se a máquina de estados do ar-condicionado:
```
void ac_run_sm(ac_sm_t *sm){

	switch(sm->state){
		case AC_INIT:
			sm->histerese_c = 2.0;
			sm->state = AC_IDLE;
			sm->set_point_c = 25.0;
			break;
		case AC_IDLE:
			delay = DELAY_IDLE;
			ac_cool_set(false);
			ac_heat_set(false);
			if(room_temp > (sm->set_point_c + sm->histerese_c))
				sm->state = AC_COOL;
			else if(room_temp < (sm->set_point_c - sm->histerese_c))
				sm->state = AC_HEAT;
			break;
		case AC_HEAT:
			delay = DELAY_HEATING;
			ac_heat_set(true);
			if(room_temp > sm->set_point_c)
				sm->state = AC_IDLE;
			break;
		case AC_COOL:
			delay = DELAY_COOLING;
			ac_cool_set(true);
			if(room_temp < sm->set_point_c)
				sm->state = AC_IDLE;
			break;
		default:
			sm->state = AC_INIT;
			break;
	}
}
```

6. Resultado:

![Circuito](https://github.com/LeslyMontufar/ac_temp/blob/main/img/circuito.png)
