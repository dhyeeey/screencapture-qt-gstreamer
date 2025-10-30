<body>
  <h1>ScreenCapture Qt GStreamer</h1>
  <p>
    <strong>ScreenCapture Qt GStreamer</strong> is a C++ project that enables high-performance screen capture in a simple window using the Qt framework and the GStreamer multimedia library. This project is suitable for developers interested in multimedia, real-time streaming, and desktop capture solutions leveraging open-source, cross-platform technologies.
  </p>
  <h2>Features</h2>
  <ul>
    <li>Real-time desktop or window screen capture</li>
    <li>Qt-based graphical user interface (GUI) controls</li>
    <li>Modular and extensible structure for additional platforms or streaming targets</li>
  </ul>
  <h2>Technologies Used</h2>
  <ul>
    <li><strong>C++</strong>: Core application logic</li>
    <li><strong>Qt</strong>: User interface and event handling</li>
    <li><strong>GStreamer</strong>: Multimedia processing and streaming</li>
    <li><strong>CMake</strong>: Platform-independent build system</li>
  </ul>
  <h2>Getting Started</h2>
  <h3>Prerequisites</h3>
  <ul>
    <li>Qt 6.2+ (recommended)</li>
    <li>GStreamer 1.26.1 runtime and development libraries</li>
    <li>CMake 3.16 or above</li>
    <li>C++17-compatible compiler (MSVC preferred)</li>
  </ul>
  <h3>Build Instructions</h3>
  <ol>
    <li>Clone the repository:<br/>
      <code>git clone https://github.com/dhyeeey/screencapture-qt-gstreamer.git</code>
    </li>
    <li>Navigate to project directory and open it using Qt creator IDE
    </li>
    <li>
      Run code using your choice of C++ compiler (MSVC preffered on windows)
    </li>
  </ol>
  <h3>Configuration</h3>
  <ul>
    <li>
      Make sure gst-launch-1.0 and plugins are in your PATH (Linux: install via package manager, Windows/macOS: install from GStreamer's official installers).
    </li>
    <li>
      On first run, configure any access permissions for desktop capture.
    </li>
  </ul>
  <h2>Customization</h2>
  <p>
    You can extend this project by integrating new GStreamer plugins, new Gstreamer pipelines, hardware encoders, or customizing the Qt GUI to support new features.
  </p>
  <h2>License</h2>
  <p>
    This project is open source and available under the MIT License.
  </p>
  <h2>Contributing</h2>
  <p>
    Contributions are encouraged! Fork the repository, create feature branches, and submit pull requests for review. Contributions for mac and linux platforms would be highly appreciated as I currently own windows machine and I dont have access to mac and windows
  </p>
  <h2>Acknowledgments</h2>
  <ul>
    <li><a href="https://www.gstreamer.net/">GStreamer project</a></li>
    <li><a href="https://www.qt.io/">Qt project</a></li>
  </ul>
</body>
