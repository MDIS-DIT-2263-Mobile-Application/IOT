import { StatusBar } from "expo-status-bar";
import React, { useEffect, useState } from "react";
import {
  Button,
  StyleSheet,
  Text,
  View,
} from "react-native";
import mqtt from "mqtt";

// =========================
// Change these
// =========================

const MQTT_SERVER = `wss://${process.env.EXPO_PUBLIC_MQTT_SERVER}:${process.env.EXPO_PUBLIC_MQTT_PORT || 8884}/mqtt`;
console.log(MQTT_SERVER);

const options = {
  username: process.env.EXPO_PUBLIC_MQTT_USER || "YOUR_USERNAME",
  password: process.env.EXPO_PUBLIC_MQTT_PASSWORD || "YOUR_PASSWORD",
  clientId: "react_" + Math.random().toString(16).substr(2, 8),
};

// =========================

export default function App() {
  const [client, setClient] = useState<any>(null);
  const [ledState, setLedState] = useState("Unknown");
  const [connected, setConnected] = useState(false);

  useEffect(() => {
    const mqttClient = mqtt.connect(MQTT_SERVER, options);

    mqttClient.on("connect", () => {
      console.log("MQTT Connected");

      setConnected(true);

      mqttClient.subscribe("esp32/led/state");
    });

    mqttClient.on("message", (topic, payload) => {
      const msg = payload.toString();

      console.log(topic, msg);

      if (topic === "esp32/led/state") {
        setLedState(msg);
      }
    });

    mqttClient.on("close", () => {
      setConnected(false);
    });

    mqttClient.on("error", (err) => {
      console.log(err);
    });

    setClient(mqttClient);

    return () => {
      mqttClient.end();
    };
  }, []);

  const toggleLED = () => {
    if (!client) return;

    if (ledState === "ON") {
      client.publish("esp32/led/set", "OFF");
    } else {
      client.publish("esp32/led/set", "ON");
    }
  };

  return (
    <View style={styles.container}>

      <Text style={styles.title}>
        ESP32 MQTT Demo
      </Text>

      <Text>
        MQTT :
        {connected ? " Connected" : " Disconnected"}
      </Text>

      <Text style={styles.status}>
        LED : {ledState}
      </Text>

      <Button
        title={ledState === "ON" ? "Turn OFF" : "Turn ON"}
        onPress={toggleLED}
      />

      <StatusBar style="auto" />

    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: "center",
    alignItems: "center",
    padding: 20,
  },

  title: {
    fontSize: 26,
    fontWeight: "bold",
    marginBottom: 30,
  },

  status: {
    marginVertical: 20,
    fontSize: 24,
    fontWeight: "bold",
  },
});