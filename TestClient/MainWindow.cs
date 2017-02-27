﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;

namespace TestClient
{
    public partial class MainWindow : Form
    {
        private TCPClient m_tcpClient;
        private Dictionary<string, NetMessage> m_NetMessages = new Dictionary<string, NetMessage>();

        public MainWindow()
        {
            InitializeComponent();

            buttonDisconnect.Visible = false;
            mainList.FullRowSelect = true;
            connectButton.Select();
            //UInt32 test = 29626497;
            //byte[] zd = BitConverter.GetBytes(test);
        }

        private void connectButton_Click(object sender, EventArgs e)
        {
            try
            {
                m_tcpClient = new TCPClient(this, ipAddress.Text, UInt16.Parse(port.Text));
            }
            catch (Exception ex)
            {
                 ShowError(ex.Message);
                 return;
            }

            ShowMessage("Connecting...");
        }

        public void OnConnect()
        {
            Debug.Assert(m_tcpClient.IsConnected);

            Invoke(new Action(() =>
            {
                buttonDisconnect.Visible = true;
                ShowMessageSucess("Connected to " + m_tcpClient.RemoteEndPoint.Address + " port: " + m_tcpClient.RemoteEndPoint.Port);

                if (writeTextBox.Text.Length > 0)
                    sendButton.Enabled = true;
            }));
        }

        public void ShowError(string error)
        {
            Invoke(new Action(() =>
            {
                connectedStatus.Text = "Connection error: " + error;
                connectedStatus.ForeColor = Color.Red;
            }));
        }

        public void ShowMessage(string msg)
        {
            Invoke(new Action(() =>
            {
                connectedStatus.ForeColor = System.Drawing.SystemColors.ControlText;
                connectedStatus.Text = msg;
            }));
        }

        public void ShowMessageSucess(string msg)
        {
            Invoke(new Action(() =>
            {
                connectedStatus.Text = msg;
                connectedStatus.ForeColor = Color.Green;
            }));
        }

        public void AddNetMessageToUI(NetMessage netMessage, int bytesTransfered)
        {
            string time = string.Format("{0:HH:mm:ss}", DateTime.Now);
            string direction;

            if (netMessage.MsgDirection == NetMessage.Direction.send)
                direction = "Send";
            else
                direction = "Received";

            string[] row = { time, direction, bytesTransfered.ToString(), netMessage.DecodeData(NetMessageEncoding.UTF8), netMessage.DecodeData(NetMessageEncoding.hex) };

            Invoke(new Action(() =>
            {
                mainList.Items.Insert(0, new ListViewItem(row));
                mainList.Items[0].Tag = netMessage;
            }));
        }

        private void buttonDisconnect_ButtonClick(object sender, EventArgs e)
        {
            m_tcpClient.Disconnect();
            m_tcpClient = null;
            buttonDisconnect.Enabled = false;
        }

        private void writeTextBox_TextChanged(object sender, EventArgs e)
        {
            if (writeTextBox.Text.Length > 0 && m_tcpClient != null && m_tcpClient.IsConnected)
            {
                sendButton.Enabled = true;
            }
        }

        private void sendButton_Click(object sender, EventArgs e)
        {
            NetMessageEncoding encoding;

            if (radioButtonHex.Checked)
                encoding = NetMessageEncoding.hex;
            else if (radioButtonUTF8.Checked)
                encoding = NetMessageEncoding.UTF8;
            else
                encoding = NetMessageEncoding.ASCII;


            NetMessage netMessage = new NetMessage(NetMessage.EncodeString(encoding, writeTextBox.Text), NetMessage.Direction.send);

            m_tcpClient.Send(netMessage);
            writeTextBox.Text = string.Empty;
            sendButton.Enabled = false;
        }

        private void mainList_SelectedIndexChanged(object sender, EventArgs e)
        {
            SetReadTextBoxContent();
        }

        private void SetReadTextBoxContent()
        {
            if (mainList.SelectedIndices.Count < 1)
                return;

            var netMessage = (NetMessage)mainList.Items[mainList.SelectedIndices[0]].Tag;

            NetMessageEncoding encoding;

            if (radioButtonReadHex.Checked)
                encoding = NetMessageEncoding.hex;
            else if (radioButtonReadUTF8.Checked)
                encoding = NetMessageEncoding.UTF8;
            else
                encoding = NetMessageEncoding.ASCII;

            readTextBox.Text = netMessage.DecodeData(encoding);
        }

        private void radioButtonReadASCII_CheckedChanged(object sender, EventArgs e)
        {
            SetReadTextBoxContent();
        }

        private void radioButtonReadUTF8_CheckedChanged(object sender, EventArgs e)
        {
            SetReadTextBoxContent();
        }

        private void radioButtonReadHex_CheckedChanged(object sender, EventArgs e)
        {
            SetReadTextBoxContent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            mainList.Items.Clear();
        }

        private void richConnectButton_Click(object sender, EventArgs e)
        {
            try
            {
				m_tcpClient = new TCPClient(this, ipAddressRich.Text, UInt16.Parse(portRich.Text));
				m_tcpClient.Send(new NetMessage(NetMessage.EncodeString(NetMessageEncoding.UTF8, "CONNECT\n" + textBoxRichUsername.Text + " " + "TODOpassword" + "\n"), NetMessage.Direction.send));

                buttonSendApoapseMsg.Enabled = true;
                sendButton.Enabled = true;
            }
            catch (Exception ex)
            {
                ShowError(ex.Message);
                return;
            }
        }

        private void buttonSendApoapseMsg_Click(object sender, EventArgs e)
        {
			var conversationUuid = Uuid.Generate();
			
			m_tcpClient.Send(new NetMessage(NetMessage.EncodeString(NetMessageEncoding.UTF8, "CONVERSATION\n" + "{\"uuid\": \"" + conversationUuid + "\", \"correspondents\": [\"" + textBoxRichDestination.Text + "\"]}" + "\n\n"), NetMessage.Direction.send));

			string dateTime = DateTime.UtcNow.ToString("yyyy-MM-ddTHH\\:mm\\:ss") + "Z";
			var msgContent = NetMessage.EncodeString(NetMessageEncoding.UTF8, apoapseMsgContent.Text);

			m_tcpClient.Send(new NetMessage(NetMessage.EncodeString(NetMessageEncoding.UTF8, "MESSAGE\n" + "{\"uuid\": \"" + Uuid.Generate() + "\", \"from\": \"apoapse.space:" + textBoxRichUsername.Text + "\", \"conversation\": \"" + conversationUuid + "\", \"sent\": \"" + dateTime + "\", \"payload_size\": " + msgContent.Count() + "}" + "\n\n" + apoapseMsgContent.Text), NetMessage.Direction.send));
		}
	}
}
