# XViewer-Opta

**XViewer-Opta** is a library designed to facilitate communication between **Arduino Opta** devices and the **XViewer** mobile application, available for both iOS and Android. This library provides a set of APIs to enable seamless integration between hardware and software.

## Features

- **Compatibility**: Specifically developed for **Arduino Opta**.
- **Communication**: Provides APIs to interact with the **XViewer** app.
- **Interface Support**: Works with both **WiFi** and **Ethernet** interfaces.
- **Ease of Use**: Simple integration into existing projects.

## Installation

To use the **XViewer-Opta** library in your Arduino project:

1. **Download**: Clone or download the latest version from the [GitHub repository](https://github.com/idt-redsmart/XViewer-Opta).
2. **Include**: Add the library to your Arduino environment.

## Usage

The library supports both **WiFi** and **Ethernet** interfaces. You can initialize the `XViewer` object as follows:

- **Using Ethernet Interface**:  
  ```cpp
  XViewer<EthernetServer, EthernetClient> xv;
  ```
  
- **Using WiFi Interface**:  
  ```cpp
  XViewer<WiFiServer, WiFiClient> xv;
  ```

For detailed usage and examples, refer to the `examples` folder in the repository.

## XViewer Mobile App

Download the **XViewer** app for your mobile device:
- **Android**: [Google Play Store](https://play.google.com/store/apps/details/Xviewer?id=com.redsmart.modulex&hl=it)
- **iOS**: [Apple App Store](https://apps.apple.com/it/app/xviewer/id6450307897)

## Contributions

Contributions are welcome! If you want to contribute:

1. Fork the repository.
2. Create a new branch (`git checkout -b feature/your-feature-name`).
3. Commit your changes (`git commit -am 'Add new feature'`).
4. Push the branch (`git push origin feature/your-feature-name`).
5. Open a Pull Request.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

## Contact

For questions or support, contact the development team at [info@idr.it](mailto:info@idr.it).

---
