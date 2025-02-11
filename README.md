# 🐾 Proyecto Mascota Virtual

¡Bienvenido al proyecto de Mascota Virtual! Este proyecto simula una mascota virtual con una pantalla OLED, un LED RGB y varios sensores y botones. La mascota puede ser revivida y jugar al baloncesto con solo presionar un botón.

## 🚀 Características

- 📟 Pantalla OLED para mostrar el estado de la mascota.
- 🌈 LED RGB que cambia de color.
- 🕹️ Botones para interactuar con la mascota.
- 📡 Conexión WiFi opcional para funcionalidades web.

## 🛠️ Instalación

1. Clona este repositorio:
    ```sh
    git clone https://github.com/JoseFornesJimenez/Mascota-RUMMI
    ```
2. Abre el proyecto en tu entorno de desarrollo preferido.
3. Asegúrate de tener instaladas las siguientes bibliotecas:
    - `Adafruit_GFX`
    - `Adafruit_SSD1306`
    - `Adafruit_NeoPixel`
    - `Adafruit_HMC5883_U`
    - `WiFi`
    - `WebServer`
    - `Preferences`

4. Conecta los componentes según el esquema de pines definido en el código.

## 📋 Uso

1. Sube el código al microcontrolador.
2. Si hay una red WiFi disponible, el dispositivo intentará conectarse. Si no, continuará sin la funcionalidad web.
3. Usa los botones para interactuar con la mascota:
    - Botón 1 (GPIO 8): Revivir la mascota.
    - Botón 2 (GPIO 3): Activar el modo baloncesto.

## 📷 Esquema de Conexión

- **Pantalla OLED**: Conectada a los pines I2C (SDA, SCL).
- **LED RGB**: Conectado al pin GPIO 20.
- **Botón 1**: Conectado al pin GPIO 8 con pull-up interno.
- **Botón 2**: Conectado al pin GPIO 3 con pull-up interno.
- **Sensor de Temperatura**: Conectado al pin A1.
- **Sensor GY-273**: Conectado a los pines I2C (SDA, SCL).

## 📄 Licencia

Este proyecto está bajo la Licencia MIT. Consulta el archivo [LICENSE](LICENSE) para más detalles.

## 🤝 Contribuciones

¡Las contribuciones son bienvenidas! Si tienes alguna idea o mejora, no dudes en abrir un issue o enviar un pull request.

## 📧 Contacto

Si tienes alguna pregunta o sugerencia, puedes contactarme en [jose.fornes.jimenez@gmail.com](mailto:jose.fornes.jimenez@gmail.com).

---

¡Gracias por visitar el proyecto de Mascota Virtual! 🐶🐱
