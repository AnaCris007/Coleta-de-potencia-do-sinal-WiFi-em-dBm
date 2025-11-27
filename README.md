# Coleta de potência do Sinal WiFi em dBm

### Vídeo da Experiência

Link do vídeo: [Google Drive](https://drive.google.com/file/d/1kpMnCqQHvTwTPDMznksUx816LzB_LY1R/view?usp=sharing)

---

## Descrição da Atividade

Este projeto consiste em um sistema IoT para monitorar e publicar em tempo real a potência do sinal WiFi (RSSI) em dBm utilizando um ESP32. O dispositivo conecta-se a uma rede WiFi, mede continuamente a intensidade do sinal, imprime os valores na porta serial da Arduino IDE e publica cada leitura em uma plataforma online(Ubidots) via MQTT. Na plataforma, uma dashboard exibe um gráfico contínuo Tempo × dBm, permitindo analisar variações de sinal em diferentes cenários, incluindo um teste que simula o efeito de uma gaiola de Faraday.

## Objetivos

- Conectar o ESP32 a uma rede WiFi e medir o RSSI em dBm.
- Exibir as leituras na porta serial da Arduino IDE.
- Publicar cada leitura via MQTT em uma plataforma online (dashboard).
- Configurar um gráfico contínuo Tempo × dBm na plataforma.
- Realizar testes em ambientes distintos e registrar os resultados.
- Executar o experimento no elevador do Inteli para observar o bloqueio temporário de sinal.

## Componentes e Ferramentas

- Hardware: ESP32
- IDE: Arduino IDE
- Protocolo: MQTT (cliente no ESP32, broker/plataforma online)
- Plataforma: Dashboard online com gráfico em tempo real (Tempo × dBm)

## Fluxo de Funcionamento

1. O ESP32 conecta-se ao WiFi e lê o RSSI atual (em dBm).
2. A cada intervalo de tempo (ex.: 1 segundo), o valor de dBm é:
   - Impresso na porta serial (monitor serial da Arduino IDE).
   - Publicado via MQTT em um tópico configurado (ex.: `iot/esp32/wifi/rssi`).
3. A plataforma online consome o tópico MQTT e atualiza o gráfico Tempo × dBm.
4. Os dados ficam disponíveis para análise e para documentar o efeito de bloqueios temporários.

## Código MQTT (conceito)

- Configurar o cliente MQTT no ESP32 (broker, porta, usuário/senha se necessário).
- Após conectar ao WiFi e ao broker, publicar periodicamente o RSSI.
- Garantir reconexão automática ao broker em caso de falhas.

> Observação: Ajuste o tópico MQTT, credenciais e endereço do broker conforme a plataforma escolhida.

## Dashboard e Visualização

- Criar na plataforma um gráfico contínuo (linha) com eixo X = Tempo e eixo Y = dBm.
- Assinar o tópico MQTT publicado pelo ESP32.
- Habilitar atualização em tempo real para observar variações instantâneas.

![Gráfico Tempo × dBm](../grafico.png)

## Experimento: Gaiola de Faraday (Elevador)

1. Leve o ESP32 até o elevador com a dashboard aberta.
2. Entre no elevador e permaneça por pelo menos 5 segundos.
3. Observe a queda acentuada (ou perda) do sinal no gráfico.
4. Saia do elevador e verifique a recuperação do sinal.
5. Certifique-se de que a dashboard registre claramente o bloqueio temporário.

## Resultados Esperados

- Quedas perceptíveis de dBm dentro do elevador, representando o bloqueio do sinal.
- Recuperação do sinal ao sair do ambiente metálico.
- Curva do gráfico mostrando variações coerentes com a distância e obstáculos.
