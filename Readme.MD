# QuickBite - Food Delivery System

A Food Delivery System built using **C (Data Structures)** for the backend and **HTML/CSS/JS** for the frontend, connected via a Python Flask bridge.

## Tech Stack

| Layer | Technology |
|---|---|
| Data Structures | C (Queue + Stack) |
| API Bridge | Python Flask |
| Frontend | HTML, CSS, JavaScript |

## Data Structures Used

- **Queue** (Circular) — manages active orders (FIFO)
- **Stack** — stores delivery history (LIFO)

## Project Structure

```
Ds Project/
├── backend/
│   ├── common.h
│   ├── queue.h / queue.c
│   ├── stack.h / stack.c
│   ├── main.c
│   └── food          ← compiled C binary
├── frontend/
│   ├── index.html
│   ├── style.css
│   └── script.js
├── server.py
└── Readme.MD
```

## How to Run

### Step 1 — Compile the C backend
```bash
cd backend
gcc main.c queue.c stack.c -o food
chmod +x food
cd ..
```

### Step 2 — Install Python dependencies
```bash
pip3 install flask flask-cors --break-system-packages
```

### Step 3 — Start the server
```bash
cd "/Users/jay/Ds Project"
python3 server.py
```

### Step 4 — Open the app
Open your browser and go to:
```
http://127.0.0.1:8080
```

> ⚠️ Do NOT open `index.html` directly in the browser — it must be served through Flask.

## How it Works

```
Browser (http://127.0.0.1:8080)
      ↕  fetch() API calls to /api/...
Python Flask (server.py) — serves frontend + bridges API
      ↕  runs compiled C binary
C Program (all queue/stack logic in C)
      ↕  saves state to backend/state.dat
```

## Known Issues & Fixes

| Issue | Fix |
|---|---|
| macOS port 5000 blocked by AirPlay/ControlCenter | Changed Flask port to 8080 |
| `file://` CORS block in browser | Frontend served through Flask at `http://127.0.0.1:8080` |
| `deliverNext is not defined` JS error | Added `deliverNext()` function in `script.js` |
| Flask not found | `pip3 install flask flask-cors --break-system-packages` |
| C binary permission denied | `chmod +x backend/food` or recompile with `gcc` |

## API Endpoints

| Method | Endpoint | Description |
|---|---|---|
| POST | `/api/enqueue` | Place a new order |
| POST | `/api/dequeue` | Deliver the next order |
| GET | `/api/queue` | Get all active orders |
| GET | `/api/history` | Get delivery history |