using System;
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

            buttonDisconnect.Visible = true;
            ShowMessageSucess("Connected to " + m_tcpClient.RemoteEndPoint.Address + " port: " + m_tcpClient.RemoteEndPoint.Port);

            if (writeTextBox.Text.Length > 0)
                sendButton.Enabled = true;
        }

        public void ShowError(string error)
        {
            connectedStatus.Text = "Connection error: " + error;
            connectedStatus.ForeColor = Color.Red;
        }

        public void ShowMessage(string msg)
        {
            connectedStatus.ForeColor = System.Drawing.SystemColors.ControlText;
            connectedStatus.Text = msg;
        }

        public void ShowMessageSucess(string msg)
        {
            connectedStatus.Text = msg;
            connectedStatus.ForeColor = Color.Green;
        }

        public void AddNetMessageToUI(NetMessage netMessage, int bytesTransfered)
        {
            string time = string.Format("{0:HH:mm:ss}", DateTime.Now);
            string direction;

            if (netMessage.MsgDirection == NetMessage.Direction.send)
                direction = "Send";
            else
                direction = "Received";

            string[] row = { time, direction, bytesTransfered.ToString(), "", netMessage.DecodeData(NetMessageEncoding.UTF8), netMessage.DecodeData(NetMessageEncoding.hex) };

            mainList.Items.Insert(0, new ListViewItem(row));
            mainList.Items[0].Tag = netMessage;
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


            NetMessage netMessage = new NetMessage(NetMessage.EncodeString(encoding, writeTextBox.Text), NetMessage.Direction.send, useApoapseTransportProtocol.Checked);

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

        private void useApoapseTransportProtocol_CheckedChanged(object sender, EventArgs e)
        {

        }

        public bool IsApoapseTransportProtocolUsed()
        {
            return useApoapseTransportProtocol.Checked;
        }
    }
}
