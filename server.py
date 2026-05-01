import subprocess
import json
from flask import Flask, jsonify, request, send_from_directory
from flask_cors import CORS
import os

app = Flask(__name__)
CORS(app)

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
C_BINARY = os.path.join(BASE_DIR, 'backend', 'food')

def run_c(args):
    try:
        cmd = [C_BINARY] + args
        print(f"Running: {cmd}")
        print(f"CWD: {os.path.join(BASE_DIR, 'backend')}")
        result = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            timeout=5,
            cwd=os.path.join(BASE_DIR, 'backend')
        )
        print(f"STDOUT: {result.stdout}")
        print(f"STDERR: {result.stderr}")
        print(f"Return code: {result.returncode}")
        output = result.stdout.strip()
        if output:
            return json.loads(output)
        err = result.stderr.strip()
        return {"status": "error", "message": err or "No output from C program"}
    except FileNotFoundError:
        return {"status": "error", "message": "C binary not found. Run: gcc main.c queue.c stack.c -o food inside backend/"}
    except json.JSONDecodeError:
        return {"status": "error", "message": f"Bad JSON from C: {result.stdout}"}
    except Exception as e:
        return {"status": "error", "message": str(e)}


# ── Serve Frontend ──
@app.route('/')
def index():
    return send_from_directory(os.path.join(BASE_DIR, 'frontend'), 'index.html')

@app.route('/<path:filename>')
def static_files(filename):
    return send_from_directory(os.path.join(BASE_DIR, 'frontend'), filename)


# ── API Routes ──
@app.route('/api/enqueue', methods=['POST'])
def enqueue():
    data = request.get_json()
    item = data.get('item', '')
    price = str(data.get('price', 0))
    return jsonify(run_c(['enqueue', item, price]))

@app.route('/api/dequeue', methods=['POST'])
def dequeue():
    return jsonify(run_c(['dequeue']))

@app.route('/api/queue', methods=['GET'])
def get_queue():
    return jsonify(run_c(['queue']))

@app.route('/api/history', methods=['GET'])
def get_history():
    return jsonify(run_c(['history']))


if __name__ == '__main__':
    print("=" * 50)
    print("  QuickBite → http://127.0.0.1:8080")
    print("  Open http://127.0.0.1:8080 in your browser")
    print("=" * 50)
    app.run(debug=True, port=8080)