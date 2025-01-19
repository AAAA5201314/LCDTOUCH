const express = require('express');
const cookieParser = require('cookie-parser');
const bodyParser = require('body-parser');

const app = express();
const port = 3000;

app.use(bodyParser.json());
app.use(cookieParser());

const sessions = {};
const SESSION_SECRET = 'your_secret_key';

// 模拟用户数据库
const users = {
  'user1': 'password1',
  'user2': 'password2'
};

// 登录接口
app.post('/login', (req, res) => {
  const { username, password } = req.body;
  if (users[username] && users[username] === password) {
    const sessionId = Math.random().toString(36).substring(2);
    sessions[sessionId] = { username };
    res.cookie('sessionId', sessionId, { httpOnly: true, secure: false, maxAge: 900000 });
    res.json({ success: true });
  } else {
    res.status(401).json({ success: false, message: 'Invalid credentials' });
  }
});

// 获取购物车内容接口
app.get('/cart', (req, res) => {
  const sessionId = req.cookies.sessionId;
  if (sessions[sessionId]) {
    const cart = sessions[sessionId].cart || [];
    res.json({ success: true, cart });
  } else {
    res.status(401).json({ success: false, message: 'Unauthorized' });
  }
});

// 添加商品到购物车接口
app.post('/cart', (req, res) => {
  const sessionId = req.cookies.sessionId;
  const { product } = req.body;
  if (sessions[sessionId]) {
    if (!sessions[sessionId].cart) {
      sessions[sessionId].cart = [];
    }
    sessions[sessionId].cart.push(product);
    res.json({ success: true });
  } else {
    res.status(401).json({ success: false, message: 'Unauthorized' });
  }
});

app.listen(port, () => {
  console.log(`Server is running on http://localhost:${port}`);
});