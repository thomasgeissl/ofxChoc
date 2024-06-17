// @ts-nocheck
import { useEffect, useRef, useState } from "react";
import reactLogo from "./assets/react.svg";
import viteLogo from "/vite.svg";
import { Box, Button, Card, Typography } from "@mui/material";

function App() {
  const [count, setCount] = useState(0);
  const [payload, setPayload] = useState<any>(null);
  const canvasRef = useRef(null);

  useEffect(() => {
    const handleTextureEvent = (event) => {
      const { data, width, height } = event;

      const byteCharacters = atob(data);
      const byteNumbers = new Array(byteCharacters.length);
      for (let i = 0; i < byteCharacters.length; i++) {
        byteNumbers[i] = byteCharacters.charCodeAt(i);
      }
      const byteArray = new Uint8Array(byteNumbers);

      const blob = new Blob([byteArray], { type: "image/png" });

      createImageBitmap(blob)
        .then((imageBitmap) => {
          renderToCanvas(imageBitmap);
        })
        .catch((error) => {
          console.error("Error creating ImageBitmap:", error);
        });
    };

    window?.ofxChoc?.addListener("texture", handleTextureEvent);
    window?.ofxChoc?.addListener("testevent", (event) => {
      setPayload(event);
    });

    return () => {
      // window?.ofxChoc?.removeListener("texture", handleTextureEvent);
    };
  }, []);

  function renderToCanvas(imageBitmap) {
    const canvas = canvasRef.current;
    const ctx = canvas.getContext("2d");

    if (!ctx) {
      console.error("2D context not supported");
      return;
    }

    // Clear the canvas
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    // Draw the image onto the canvas
    ctx.drawImage(imageBitmap, 0, 0, canvas.width, canvas.height);
  }

  return (
    <Box
      width={"100vw"}
      height={"100vh"}
      display={"flex"}
      flexDirection={"column"}
      gap={3}
    >
      <Typography variant="h1">Vite + React + ofxChoc</Typography>
      <Box>
        <Button
          variant="outlined"
          onClick={() => {
            setCount((count) => count + 1);
            window.ofxChoc?.notifyEvent("randombg");
          }}
        >
          randombg
        </Button>
      </Box>
      <Card>
        <pre>{JSON.stringify(payload, null, 4)}</pre>
      </Card>
      <Box flex={1} display={"flex"}>
        <canvas
          ref={canvasRef}
          id="canvas"
          style={{ margin: "auto" }}
        ></canvas>
      </Box>
    </Box>
  );
}

export default App;
