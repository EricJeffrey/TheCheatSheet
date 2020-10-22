import React from 'react';
import { Button, Card, Input, Layout, Menu } from 'antd';
import 'antd/dist/antd.css';
import './css/App.css';

const { Header, Sider, Content } = Layout;

function App() {
    return (
        <Layout className="comon_theme" id="main">
            <Header className="comon_theme" id="main_header">
                <a href="/" id="main_logo">TheCheatsheet</a>
                <div id="main_search_holder">
                    <Input id="main_input_search" placeholder="search"></Input>
                    <div className="middle_space"></div>
                    <Button id="main_btn_add">添加</Button>
                </div>
            </Header>
            <Layout style={{ width: "100%" }}>
                <Sider className="comon_theme" id="main_sider" theme="light">
                    <div className="middle_space"></div>
                    <Menu id="main_menu" selectedKeys="2">
                        <Menu.Item key="1">Bash</Menu.Item>
                        <Menu.Item key="2">Java</Menu.Item>
                        <Menu.Item key="3">C++</Menu.Item>
                        <Menu.Item key="4">Python</Menu.Item>
                    </Menu>
                </Sider>
                <Content className="comon_theme" id="main_content" >
                    <Card>
                        <p>Linux创建空文件</p>
                        <p>创建不包含任何内容的空文件，echo &gt; a.txt有相同功能</p>
                        <p>touch a.txt</p>
                    </Card>
                </Content>
            </Layout>
        </Layout>
    );
}

export default App;
