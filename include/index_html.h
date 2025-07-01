#ifndef INDEX_HTML_H
#define INDEX_HTML_H

const char *index_html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Chat</title>
  <style>
    body { font-family: Arial, sans-serif; margin: 20px; }
    #chatBox { width: 100%; height: 300px; border: 1px solid #ccc; overflow-y: scroll; padding: 10px; }
    #messageInput { width: 80%; padding: 5px; }
    button { padding: 5px 10px; }
  </style>
</head>
<body>
  <h1>ESP32 BLE & Web Chat</h1>
  <div id="chatBox"></div>
  <input type="text" id="messageInput" placeholder="Type a message...">
  <button onclick="sendMessage()">Send</button>
  <script>
    const ws = new WebSocket('ws://' + window.location.hostname + '/ws');
    const chatBox = document.getElementById('chatBox');
    const messageInput = document.getElementById('messageInput');

    ws.onmessage = function(event) {
      const message = document.createElement('div');
      message.textContent = event.data;
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