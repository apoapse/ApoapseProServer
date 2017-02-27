using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestClient
{
	struct Uuid
	{
		public static string Generate()
		{
			string strOut = System.Guid.NewGuid().ToString();
			return strOut.Replace("-", String.Empty);
		}
	}
}
