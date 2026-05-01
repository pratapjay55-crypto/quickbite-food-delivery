var API = 'http://127.0.0.1:8080/api';;
var toastTimer;

var emojiMap = { 'Burger': '🍔', 'Pizza': '🍕', 'Sandwich': '🥪' };

function placeOrder(item, price) {
  fetch(API + '/enqueue', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ item: item, price: price })
  })
  .then(function(r) { return r.json(); })
  .then(function(data) {
    if (data.status === 'ok') {
      showToast(item + ' placed! ' + data.deliveryBoy + ' is on the way. 🛵');
      refreshAll();
    } else {
      showToast(data.message || 'Error placing order!');
    }
  })
  .catch(function() { showToast('❌ Cannot connect to server. Is server.py running?'); });
}

function deliverNext() {
  fetch(API + '/dequeue', { method: 'POST' })
  .then(function(r) { return r.json(); })
  .then(function(data) {
    if (data.status === 'ok') {
      showToast('✅ Delivered: ' + data.item + ' by ' + data.deliveryBoy);
      refreshAll();
    } else {
      showToast(data.message || 'No orders in queue!');
    }
  })
  .catch(function() { showToast('❌ Cannot connect to server. Is server.py running?'); });
}

function deliverOrder() {
  deliverNext();
}

function refreshAll() {
  refreshQueue();
  refreshHistory();
}

function refreshQueue() {
  fetch(API + '/queue')
  .then(function(r) { return r.json(); })
  .then(function(data) {
    var body = document.getElementById('queue-body');
    var empty = document.getElementById('queue-empty');
    var cards = body.querySelectorAll('.order-card');
    for (var i = 0; i < cards.length; i++) cards[i].remove();

    if (!data.orders || data.orders.length === 0) {
      empty.style.display = 'block';
      updateStats(0);
      return;
    }
    empty.style.display = 'none';
    updateStats(data.orders.length);

    for (var i = 0; i < data.orders.length; i++) {
      var o = data.orders[i];
      var emoji = emojiMap[o.item] || '🍽️';
      var card = document.createElement('div');
      card.className = 'order-card';
      card.innerHTML =
        '<div class="order-num">' + (i + 1) + '</div>' +
        '<div style="font-size:22px">' + emoji + '</div>' +
        '<div class="order-info">' +
          '<div class="order-name">' + o.item + '</div>' +
          '<div class="order-boy"><span class="boy-badge">' + o.deliveryBoy + '</span></div>' +
        '</div>' +
        '<div class="order-price">Rs.' + o.price + '</div>' +
        (i === 0
          ? '<button class="deliver-btn" onclick="deliverOrder()">Deliver</button>'
          : '<span class="deliver-btn" style="opacity:0.3;cursor:default">Waiting</span>');
      body.appendChild(card);
    }
  })
  .catch(function() {});
}

function refreshHistory() {
  fetch(API + '/history')
  .then(function(r) { return r.json(); })
  .then(function(data) {
    var body = document.getElementById('history-body');
    var empty = document.getElementById('history-empty');
    var cards = body.querySelectorAll('.history-card');
    for (var i = 0; i < cards.length; i++) cards[i].remove();

    if (!data.history || data.history.length === 0) {
      empty.style.display = 'block';
      document.getElementById('stat-delivered').textContent = '0';
      document.getElementById('stat-revenue').textContent = 'Rs.0';
      return;
    }
    empty.style.display = 'none';

    var total = 0;
    for (var i = 0; i < data.history.length; i++) {
      total += data.history[i].price;
      var o = data.history[i];
      var emoji = emojiMap[o.item] || '🍽️';
      var card = document.createElement('div');
      card.className = 'history-card';
      card.innerHTML =
        '<div class="history-item-name">' + emoji + ' ' + o.item + '</div>' +
        '<div class="history-meta"><span>' + o.deliveryBoy + '</span><span>Rs.' + o.price + '</span></div>';
      body.appendChild(card);
    }

    document.getElementById('stat-delivered').textContent = data.history.length;
    document.getElementById('stat-revenue').textContent = 'Rs.' + total;
  })
  .catch(function() {});
}

function updateStats(queueLen) {
  document.getElementById('stat-queued').textContent = queueLen;
}

function showToast(msg) {
  var t = document.getElementById('toast');
  t.textContent = msg;
  t.classList.add('show');
  clearTimeout(toastTimer);
  toastTimer = setTimeout(function() { t.classList.remove('show'); }, 2500);
}

window.onload = function() { refreshAll(); };