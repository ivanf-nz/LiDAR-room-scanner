# LiDAR Room Scanner

## 🧪 Development Status & Planned Improvements

> ⚠️ **Note:** This project is currently under active development. Although it's currently titled **"realtime-mesh-render,"** it doesn't yet perform real-time mesh rendering. The system captures 3D point cloud data in real time and saves it to a `.xyz` file, which can then be manually loaded into software like MeshLab for visualization. Real-time rendering and mesh generation features are in progress and will be added in future updates.

### 🔧 What’s Coming Next

Here are some planned improvements and features currently being worked on:

- **Bounding Box Filtering:** Add logic to automatically discard invalid or noisy distance readings during scanning.
- **Mesh Export Support:** Enable the scanner to export directly to common 3D formats like `.PLY` or `.STL`.
- **Automated Post-Processing:** Use Python scripts (e.g. with Open3D) to automatically generate a mesh from the `.xyz` point cloud.
- **Real-Time Visualization (Planned):** Work is underway to support live rendering of the scan as it happens.
- **Enhanced Accuracy:** Fine-tune motor steps and sensor positioning for more precise scanning and smoother meshes.
- **IOT Device:** Allow for data transfer over bluetooth or wifi to allow the device to be placed anywhere along with a battery.
---

## 🧾 **Current Functionality:**  
This project is a complete room-scanning setup using a stepper motor-based LiDAR mount. It integrates an Arduino with two NEMA 17 stepper motors, a TF Luna LiDAR sensor, and Hall effect sensors for homing. A Python script captures the resulting point cloud data over serial and logs it to an `.xyz` file for later 3D mesh reconstruction.
<h3>📸 Project Setup</h3>

<img src="https://raw.githubusercontent.com/ivanf-nz/realtime-mesh-renderer/refs/heads/main/readme_images/lidar_assembly.jpeg" alt="project" width="33%"/>

<h3>🗺️ MeshLab Output</h3>

<img src="https://raw.githubusercontent.com/ivanf-nz/realtime-mesh-renderer/refs/heads/main/readme_images/meshlab.jpeg" alt="meshlab" width="33%"/>

---

## ⚙️ Hardware Components

- 2 × NEMA 17 stepper motors
- 2 × Hall effect sensors (KY-003) for motor homing
- 1 × TF Luna LiDAR sensor (I2C)
- Arduino-compatible microcontroller
- 2 x Motor drivers (a4988 or any other one compatible with AccelStepper library)
- 12V, 2A Power supply & wiring

---

## 🔌 Wiring Overview

- **Stepper 1 (Rotation Base)**: Connected to pins 2 (DIR) and 3 (STEP)
- **Stepper 2 (Tilt)**: Connected to pins 4 (DIR) and 5 (STEP)
- **Hall Effect Sensors**: Connected to pins 6 and 7
- **TF Luna LiDAR**: Connected via I2C (SDA/SCL)

---

## 🧠 Arduino Logic

The Arduino performs the following tasks:

1. Initializes motors, sensors, and the LiDAR.
2. Homes both motors using Hall effect sensors.
3. Iterates through a grid of angles (base and tilt) to collect distance readings.
4. Sends the data over serial in the format:

```
Angle1 Angle2 Distance
```

Where:
- `Angle1` is the horizontal base rotation angle.
- `Angle2` is the vertical tilt angle.
- `Distance` is the LiDAR reading in centimeters.

---

## 🐍 Python Serial Logger

The Python script connects to the Arduino via serial, waits for a `READY` signal, and then logs incoming lines in the format:

```
x y z
```

Only valid lines with three numeric values are logged to the `.xyz` file for 3D point cloud visualization.

---

## 🧾 Output File: `lidar_data3.xyz`

Each line represents a 3D point in space:

```
x y z
```

You can load this file into:

- **MeshLab**
- **Or any compatible software**

---

## 🧑‍💻 Authors

Created by Ivan F and Chris L.
