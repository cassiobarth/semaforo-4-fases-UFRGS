# Semáforo de 4 Fases — Arduino Mega 2560

Disciplina: PGIE024 – Seminário Avançado: Projeto de Robótica Educacional  
Aplicado à Educação Especial na Perspectiva Inclusiva (turma RE)  
Prof. Dr. Renato Ventura Bayan Henriques

## Descrição
Controle de semáforo com 4 vias (N/S/L/O), LEDs de pedestre,
botões com interrupção e debounce, fase amarela e rotina de travessia.

## Simulação online
Você pode testar o funcionamento do código e a montagem do circuito diretamente no simulador Wokwi através deste link:
https://wokwi.com/projects/460580681039618049

## Hardware
- Arduino Mega 2560
- 4 semáforos de carros (cátodo comum)
- 4 semáforos de pedestres (anodo comum)
- 4 botões de pedestre (pinos 2, 3, 18, 19)

## Como usar
1. Carregue `semaforo/semaforo.ino` na IDE do Arduino
2. Abra o Serial Monitor em 9600 baud
3. Digite N, L, S ou O para controlar as fases
4. Pressione o botão para acionar a travessia de pedestres

## Foto
![Semáforo](media/foto_semaforo.jpg)
