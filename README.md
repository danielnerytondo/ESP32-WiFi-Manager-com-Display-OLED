# Criar o arquivo README.md
echo "# ESP32 WiFi Manager com Display OLED SH1106

![ESP32](https://img.shields.io/badge/ESP32-DevKit--v1-blue.svg)
![PlatformIO](https://img.shields.io/badge/PlatformIO-Compatible-brightgreen.svg)
![WiFiManager](https://img.shields.io/badge/WiFiManager-TZAPU-orange.svg)

Projeto completo de configuração WiFi automática para ESP32 com display OLED integrado.

## ✨ Funcionalidades

- 🔄 Conexão automática a redes WiFi salvas
- 🌐 Portal de configuração web integrado
- 📟 Display OLED para feedback visual
- 🔘 Botão físico para reset de configurações
- 💡 LED indicador de status

## 🛠️ Hardware Necessário

| Componente | Quantidade |
|------------|------------|
| ESP32 DevKit v1 | 1 |
| Display OLED 128x64 (SH1106) | 1 |
| Botão tactile switch | 1 |
| Protoboard | 1 |
| Fios jumper | Vários |

## 🔌 Esquema de Ligação

| Componente | Pino ESP32 | Observação |
|------------|------------|------------|
| OLED SDA | GPIO 21 | |
| OLED SCL | GPIO 22 | |
| Botão AP | GPIO 23 | Com pull-up interno |
| LED Interno | GPIO 2 | Já incluído no ESP32 |

![Esquema de Ligação](https://github.com/danielnerytondo/ESP32-WiFi-Manager-com-Display-OLED/blob/main/wiring_diagram.png)

## 🚀 Instalação

### Pré-requisitos
- [Visual Studio Code](https://code.visualstudio.com/)
- [PlatformIO Extension](https://platformio.org/platformio-ide)

### Clonar e Configurar

\`\`\`bash
# Clonar repositório
git clone https://github.com/MOCBUS/ESP32-WiFi-Manager-OLED.git
cd ESP32-WiFi-Manager-OLED

# Abrir no VSCode
code .
\`\`\`

### Compilar e Upload

1. Conecte o ESP32 via USB
2. No VSCode, clique em ✔️ (Build)
3. Após compilar, clique em → (Upload)
4. Monitor serial: 🔍 (Serial Monitor)

## 📖 Como Usar

1. **Primeira execução**: O ESP32 criará um AP \"ESP32_Config\"
2. **Conectar**: Conecte-se ao WiFi \"ESP32_Config\" (senha: 12345678)
3. **Configurar**: Acesse http://192.168.4.1
4. **Selecionar rede**: Escolha sua rede WiFi e insira a senha
5. **Pronto**: Credenciais salvas para conexões futuras

### Reset de Configurações
Mantenha o botão pressionado durante a inicialização para resetar as configurações WiFi.

## 🏗️ Estrutura do Projeto

\`\`\`
ESP32-WiFi-Manager-OLED/
├── src/
│   └── main.cpp          # Código principal
├── include/              # Arquivos de cabeçalho
├── platformio.ini        # Configuração PlatformIO
├── .gitignore           # Arquivos ignorados pelo Git
└── README.md            # Este arquivo
\`\`\`

## ⚙️ Configuração

### platformio.ini
\`\`\`ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200

lib_deps = 
    tzapu/WiFiManager @ ^0.16.0
    adafruit/Adafruit GFX Library
    adafruit/Adafruit SH110X
    adafruit/Adafruit BusIO

build_flags = -D WM_ESP32
\`\`\`

### Personalização
Edite no código:
\`\`\`cpp
char nomeWifi[40] = \"Seu_ESP32\";      // Nome do AP
char senhaWifi[40] = \"Sua_Senha\";     // Senha do AP
#define PIN_AP 23                     // Pino do botão
#define LED_BUILTIN 2                 // Pino do LED
\`\`\`

## 📊 Bibliotecas

- **WiFiManager**: Portal web de configuração
- **Adafruit SH110X**: Driver display OLED
- **Adafruit GFX**: Gráficos para display
- **Adafruit BusIO**: Comunicação I2C/SPI

## 🐛 Solução de Problemas

### Erros Comuns
1. **Display não encontrado**: Verifique conexões I2C
2. **WiFi não conecta**: Verifique credenciais
3. **Botão não funciona**: Verifique pull-up interno

### PlatformIO
\`\`\`bash
# Limpar cache se necessário
platformio lib uninstall --all
platformio lib update
\`\`\`

## 🤝 Contribuindo

1. Fork o projeto
2. Crie uma branch: \`git checkout -b feature/nova-feature\`
3. Commit: \`git commit -am 'Adiciona nova feature'\`
4. Push: \`git push origin feature/nova-feature\`
5. Abra um Pull Request

## 📄 Licença

Este projeto está sob licença MIT. Veja [LICENSE](LICENSE) para detalhes.

## 🙋‍♂️ Suporte

Se encontrar problemas:
1. Verifique as conexões do hardware
2. Confirme as bibliotecas instaladas
3. Abra uma issue no GitHub

---

**Desenvolvido por Daniel Felipe Nery Tondo para a comunidade ESP32**

⭐️ Dê uma estrela no repositório se este projeto te ajudou!
" > README.md
