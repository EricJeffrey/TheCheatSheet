import React from 'react';
import { Button, Card, Input, Layout, Menu, Select, Space } from 'antd';
import { EyeOutlined, CopyOutlined, EditOutlined, CloseCircleOutlined } from '@ant-design/icons';
import 'antd/dist/antd.css';
import './css/App.css';
import Modal from 'antd/lib/modal/Modal';
import { Option } from 'antd/lib/mentions';
import TextArea from 'antd/lib/input/TextArea';

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
                    <Menu id="main_menu" selectedKeys="2">
                        <Menu.Item key="1">Bash</Menu.Item>
                        <Menu.Item key="2">Java</Menu.Item>
                        <Menu.Item key="3">C++</Menu.Item>
                        <Menu.Item key="4">Python</Menu.Item>
                    </Menu>
                </Sider>
                <Content className="comon_theme" id="main_content" >
                    <Card className="main_card"
                        title="Linux创建空文件"
                        extra={
                            <Space>
                                <EyeOutlined />
                                <CopyOutlined />
                            </Space>
                        }>
                        <p>创建不包含任何内容的空文件，echo &gt; a.txt有相同功能</p>
                        <pre>touch a.txt</pre>
                    </Card>
                    <Card className="main_card"
                        title="Java日期格式化"
                        extra={
                            <Space>
                                <EyeOutlined />
                                <CopyOutlined />
                            </Space>
                        }>
                        <p>使用SimpleDateFormat类格式化当前日期并打印，日期格式为xxxx年xx月xx日星期x xx点xx分xx秒</p>
                        <pre>Date now = new Date(); // 创建一个Date对象，获取当前时间<br />
                        SimpleDateFormat f = new SimpleDateFormat("yyyy年MM月dd日 HH点mm分ss秒");<br />
                System.out.println(f.format(now)); // 将当前时间袼式化为指定的格式</pre>
                    </Card>
                </Content>
                <Modal className="main_modal" visible={true} closable={false}>
                    <div className="main_modal_content">
                        <Space style={{
                            width: "100%",
                            textAlign: "end",
                            display: "inline-flex",
                            justifyContent: "flex-end",
                            marginBottom: "1em"
                        }}>
                            <EditOutlined />
                            <CloseCircleOutlined />
                        </Space>
                        <div className="comon_theme"
                            style={{
                                marginBottom: "1em"
                            }}>
                            <Input value="Java日期格式化" />
                            <div className="small_space" />
                            <Select defaultValue="Java">
                                <Option value="php">php</Option>
                                <Option value="bash">bash</Option>
                                <Option value="bash">Java</Option>
                            </Select>
                        </div>
                        <TextArea style={{ marginBottom: "1em" }} value="placeholder">
                        </TextArea>
                        <div style={{ marginBottom: "1em" }}>
                            Date now = new Date(); // 创建一个Date对象，获取当前时间<br />
                            SimpleDateFormat f = new SimpleDateFormat("yyyy年MM月dd日 HH点mm分ss秒");<br />
                            System.out.println(f.format(now)); // 将当前时间袼式化为指定的格式<br />
                        </div>
                    </div>
                </Modal>
            </Layout>
        </Layout>
    );
}

export default App;
