#ifndef INDEX_HTML_H
#define INDEX_HTML_H

const char *index_html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 Chat</title>
  <style>
    :root {
      --primary-color: #007AFF;
      --background-color: #F2F2F7;
      --card-background: #FFFFFF;
      --text-color: #1C2526;
      --secondary-text: #6C757D;
    }

    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
    }

    body {
      font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
      background-color: var(--background-color);
      color: var(--text-color);
      padding: 20px;
      display: flex;
      justify-content: center;
      align-items: center;
      min-height: 100vh;
    }

    .container {
      max-width: 600px;
      width: 100%;
      display: flex;
      flex-direction: column;
      gap: 20px;
    }

    h1 {
      font-size: 24px;
      font-weight: 600;
      text-align: center;
      color: var(--text-color);
    }

    #chatBox {
      background-color: var(--card-background);
      border-radius: 12px;
      box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);
      height: 400px;
      overflow-y: auto;
      padding: 15px;
      display: flex;
      flex-direction: column;
      gap: 10px;
    }

    .message {
      padding: 10px 15px;
      border-radius: 12px;
      max-width: 80%;
      line-height: 1.4;
      animation: slideIn 0.3s ease-out;
    }

    .message.web {
      background-color: var(--primary-color);
      color: white;
      align-self: flex-end;
      margin-left: auto;
    }

    .message.ble {
      background-color: #E5E5EA;
      color: var(--text-color);
      align-self: flex-start;
    }

    .input-container {
      display: flex;
      gap: 10px;
      background-color: var(--card-background);
      padding: 10px;
      border-radius: 12px;
      box-shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
    }

    #messageInput {
      flex: 1;
      padding: 10px;
      border: none;
      border-radius: 8px;
      background-color: #F2F2F7;
      font-size: 16px;
      outline: none;
    }

    button {
      padding: 10px 20px;
      background-color: var(--primary-color);
      color: white;
      border: none;
      border-radius: 8px;
      font-size: 16px;
      font-weight: 500;
      cursor: pointer;
      transition: background-color 0.2s;
    }

    button:hover {
      background-color: #005BBB;
    }

    @keyframes slideIn {
      from {
        opacity: 0;
        transform: translateY(10px);
      }
      to {
        opacity: 1;
        transform: translateY(0);
      }
    }

    @media (max-width: 480px) {
      body {
        padding: 10px;
      }

      h1 {
        font-size: 20px;
      }

      #chatBox {
        height: 300px;
      }

      .input-container {
        flex-direction: column;
      }

      button {
        width: 100%;
      }
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>ESP32 BLE & Web Chat</h1>
    <div id="chatBox"></div>
    <div class="input-container">
      <input type="text" id="messageInput" placeholder="Type a message...">
      <button onclick="sendMessage()">Send</button>
    </div>
  </div>
  <script>
    const ws = new WebSocket('ws://' + window.location.hostname + '/ws');
    const chatBox = document.getElementById('chatBox');
    const messageInput = document.getElementById('messageInput');

    ws.onmessage = function(event) {
      const message = document.createElement('div');
      message.textContent = event.data;
      message.className = 'message ' + (event.data.startsWith('Web: ') ? 'web' : 'ble');
      chatBox.appendChild(message);
      chatBox.scrollTop = chatBox.scrollHeight;
    };

    function sendMessage() {
      const message = messageInput.value.trim();
      if (message) {
        ws.send(message);
        messageInput.value = '';
      }
    }

    messageInput.addEventListener('keypress', function(e) {
      if (e.key === 'Enter') {
        sendMessage();
      }
    });
  </script>
</body>
</html>
)rawliteral";

#endif