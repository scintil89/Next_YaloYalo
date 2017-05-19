using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PacketGenerator
{
	class Program
	{
		static string hCode = "";       // .h 파일 생성용
        static string cppCode = "";     // .cpp 파일 생성용
		static string csharpCode = "";  // .cs 파일 생성용
		static string line = "";

		static string GetCppDecl(string type, string name, bool isArray)
		{
			string retVal = "";
			if (isArray)
				retVal += "std::vector<";
			#region SetType
			switch (type)
			{
				case "integer":
					retVal += "int";
					break;
				case "string":
					retVal += "std::string";
					break;
				case "unsigned":
					retVal += "unsigned";
					break;
				case "bool":
					retVal += "bool";
					break;
				case "float":
					retVal += "float";
					break;
				default:
					break;
			}
			#endregion
			if (isArray)
				retVal += ">";
			else
				retVal += "\t\t\t\t";
			#region AddTab
			switch (type)
			{
				case "integer":
					retVal += "\t\t\t";
					break;
				case "string":
					retVal += "\t";
					break;
				case "unsigned":
					retVal += "\t";
					break;
				case "bool":
					retVal += "\t\t";
					break;
				case "float":
					retVal += "\t\t";
					break;
				default:
					break;
			}
			#endregion

			retVal += name + ';';
			return retVal;
		}

        static string GetCppSerializeImpl(string type, string name, bool isArray)
        {
            string ret = "";

            ret += "root[\"" + name + "\"] = " + name + ";";
            //TODO: 배열로 오면 하나씩 뜯어서 Json::Value 안에 넣어서 그걸 대입해줘야 함

            return ret;
        }

        static string GetCppDeserializeImpl(string type, string name, bool isArray)
        {
            string ret = "";
            string type_default = "";
            string type_func = "";

            switch (type)
            {
                case "integer":
                    type_default = "0";
                    type_func = "asInt()";
                    break;
                case "string":
                    type_default = "\"\"";
                    type_func = "asString()";
                    break;
                case "unsigned":
                    type_default = "0";
                    type_func = "asInt()";
                    break;
                case "bool":
                    type_default = "false";
                    type_func = "asBool()";
                    break;
                case "float":
                    type_default = "0.0";
                    type_func = "asDouble()";
                    break;
                default:
                    break;
            }

            ret += name + " = root.get(\"" + name + "\", " + type_default + ")." + type_func + ";";
            //TODO: 배열로 오면 하나씩 뜯어서 그걸 vector 에 push 해줘야 함

            return ret;
        }

        static string GetCsharpType(string type, string name, bool isArray)
		{
			string retVal = "";
			#region SetType
			switch (type)
			{
				case "integer":
					retVal = "int";
					break;
				case "string":
					retVal = "string";
					break;
				case "unsigned":
					retVal = "uint";
					break;
				case "bool":
					retVal = "bool";
					break;
				case "float":
					retVal += "float";
					break;
				default:
					break;
			}
			#endregion
			if (isArray)
				retVal += "[]";
			else
				retVal += '\t';
			#region AddTab
			switch (type)
			{
				case "integer":
					retVal += "\t\t\t\t";
					break;
				case "string":
					retVal += "\t\t\t";
					break;
				case "unsigned":
					retVal += "\t\t\t";
					break;
				case "bool":
					retVal += "\t\t\t";
					break;
				case "float":
					retVal += "\t\t\t";
					break;
				default:
					break;
			}
			#endregion
			retVal += name + ';';
			return retVal;
		}

        static string GetHIncludes(string location)
        {
            string ret = "";

            ret += "#pragma once\n\n";
            ret += "#include <string>\n";
            ret += "#include <vector>\n";
            ret += "#include \"" + location + "\"\n";
            ret += "\n";

            return ret;
        }

        static string GetHSerializeClassStr()
        {
            string ret = "";

            ret += "\tclass IJsonSerializable\n";
            ret += "\t{\n";
            ret += "\tpublic:\n";
            ret += "\t\tvirtual ~IJsonSerializable( void ) {};\n";
            ret += "\t\tvirtual void Serialize( Json::Value& root ) =0;\n";
            ret += "\t\tvirtual void Deserialize( Json::Value& root ) =0;\n";
            ret += "\t};\n\n";

            ret += "\tclass CJsonSerializer\n";
            ret += "\t{\n";
            ret += "\tpublic:\n";
            ret += "\t\tstatic bool Serialize( IJsonSerializable* pObj, std::string& output );\n";
            ret += "\t\tstatic bool Deserialize( IJsonSerializable* pObj, std::string& input );\n";
            ret += "\n";
            ret += "\tprivate:\n";
            ret += "\t\tCJsonSerializer( void ) {};\n";
            ret += "\t};\n";
            ret += "\n";

            return ret;
        }

        static string GetInheriedFunctions(string name)
        {
            string ret = "";

            ret += "\tpublic:\n";
            ret += "\t\t" + name + "( void ) {}\n";
            ret += "\t\tvirtual ~" + name + "( void ) {}\n";
            ret += "\t\tvirtual void Serialize ( Json::Value& root );\n";
            ret += "\t\tvirtual void Deserialize( Json::Value& root );\n";
            ret += "\n";

            return ret;
        }

        static string GetCppSerializerImpl()
        {
            string ret = "";

            ret += "\tbool CJsonSerializer::Serialize( IJsonSerializable* pObj, std::string& output )\n";
            ret += "\t{\n";
            ret += "\t\tif (pObj == NULL)\n";
            ret += "\t\t\treturn false;\n";
            ret += "\n";
            ret += "\t\tJson::Value serializeRoot;\n";
            ret += "\t\tpObj->Serialize(serializeRoot);\n";
            ret += "\n";
            ret += "\t\tJson::StyledWriter writer;\n";
            ret += "\t\toutput = writer.write( serializeRoot );\n";
            ret += "\n";
            ret += "\t\treturn true;\n";
            ret += "\t}\n";
            ret += "\n";

            ret += "\tbool CJsonSerializer::Deserialize( IJsonSerializable* pObj, std::string& input )\n";
            ret += "\t{\n";
            ret += "\t\tif (pObj == NULL)\n";
            ret += "\t\t\treturn false;\n";
            ret += "\n";
            ret += "\t\tJson::Value deserializeRoot;\n";
            ret += "\t\tJson::Reader reader;\n";
            ret += "\n";
            ret += "\t\tif ( !reader.parse(input, deserializeRoot) )\n";
            ret += "\t\t\treturn false;\n";
            ret += "\n";
            ret += "\t\tpObj->Deserialize(deserializeRoot);\n";
            ret += "\n";
            ret += "\t\treturn true;\n";
            ret += "\t}\n";
            ret += "\n";

            return ret;
        }

        static void Main(string[] args)
		{
			if (args.Length == 0)
			{
				Console.WriteLine("Need filename");
				return;
			}
            if (args.Length == 2)
            {
                Console.WriteLine("Need json.h location");
            }
			string filename = args[0].Split('.')[0];
			if (args[0].Split('.').Last() != "rpg")
			{
				Console.WriteLine("Only .rpg file can be consumed");
				return;
			}
			System.IO.StreamReader file = new System.IO.StreamReader(args[0]);

			bool isInClass = false;
			List<string> packetNames = new List<string>();
            List<string> memberNames = new List<string>();
            hCode += GetHIncludes(args[1]);
			hCode += "namespace " + filename + "\n{\n";
            hCode += GetHSerializeClassStr();

            cppCode += "#include \"" + filename + ".h\"\n\n";
            cppCode += "namespace " + filename + "\n{\n";
            cppCode += GetCppSerializerImpl();

			csharpCode += "namespace " + filename + "\n{\n";

			while ((line = file.ReadLine()) != null)
			{
				char[] tokens = { ' ', '\t' };
				string[] sliced = line.Split(tokens, StringSplitOptions.RemoveEmptyEntries);
				if (sliced.Length == 0)
					continue;

				// 주석 처리
				if (sliced[0].StartsWith("//"))
				{
					hCode += '\t' + line + '\n';
					csharpCode += '\t' + line + '\n';
					continue;
				}
				if (!isInClass)
				{
					isInClass = true;
					hCode += "\tclass " + line + " : public IJsonSerializable\n";

					csharpCode += "\tpublic class " + line + '\n';

					// 클래스명 목록에 추가
					packetNames.Add(line);
					continue;
				}

				if (line.StartsWith("{"))
				{
					hCode += "\t{\n";

                    if(isInClass)
                    {
                        hCode += GetInheriedFunctions(packetNames[packetNames.Count - 1]);
                    }

					csharpCode += "{\n";
					continue;
				}

				if (line.StartsWith("}"))
				{
                    // 클래스를 빠져나오려고 한다면..
                    if(isInClass)
                    {
                        cppCode += "\tvoid " + packetNames[packetNames.Count - 1] + "::Serialize( Json::Value& root )\n\t{\n";

                        // 멤버 정보 생성
                        foreach (string member in memberNames)
                        {
                            hCode += "\t\t";
                            cppCode += "\t\t";
                            csharpCode += "\t";
                            
                            string[] vars = member.Split(tokens, StringSplitOptions.RemoveEmptyEntries);

                            switch (vars[0])
                            {
                                case "single":
                                    hCode += GetCppDecl(vars[1], vars[2], false);
                                    cppCode += GetCppSerializeImpl(vars[1], vars[2], false);
                                    csharpCode += "public " + GetCsharpType(vars[1], vars[2], false);
                                    break;
                                case "array":
                                    hCode += GetCppDecl(vars[1], vars[2], true);
                                    cppCode += GetCppSerializeImpl(vars[1], vars[2], true);
                                    csharpCode += "public " + GetCsharpType(vars[1], vars[2], true);
                                    break;
                            }

                            hCode += "\n";
                            cppCode += "\n";
                            csharpCode += "\n";
                        }

                        cppCode += "\t}\n\n";
                        cppCode += "\tvoid " + packetNames[packetNames.Count - 1] + "::Deserialize( Json::Value& root )\n\t{\n";

                        // cpp 파일만 한 번 더 써줘야 함
                        foreach (string member in memberNames)
                        {
                            cppCode += "\t\t";

                            string[] vars = member.Split(tokens, StringSplitOptions.RemoveEmptyEntries);

                            switch (vars[0])
                            {
                                case "single":
                                    cppCode += GetCppDeserializeImpl(vars[1], vars[2], false);
                                    break;
                                case "array":
                                    cppCode += GetCppDeserializeImpl(vars[1], vars[2], true);
                                    break;
                            }

                            cppCode += "\n";
                        }

                        cppCode += "\t}\n\n";

                        memberNames.Clear();
                    }

                    isInClass = false;

					hCode += "\t};\n\n";
					csharpCode += "\t}\n\n";
					continue;
				}

                // 멤버 선언
                memberNames.Add(line);
			}

            // 패킷번호 생성
            hCode += "\tenum PacketId\n\t{\n";
			csharpCode += "\tpublic enum PacketId\n\t{\n";

			uint counter = 101;
			foreach (string packetName in packetNames)
			{
				hCode += "\t\tID_" + packetName + "\t\t\t= " + counter + ",\n";
				csharpCode += "\t\tID_" + packetName + "\t\t\t= " + counter + ",\n";
				counter++;
			}
			hCode += "\t};\n";
			csharpCode += "\t};\n";

			// 네임스페이스 닫기
			hCode += '}';
            cppCode += '}';
			csharpCode += '}';

			byte[] hByte = Encoding.GetEncoding("ks_c_5601-1987").GetBytes(hCode.ToCharArray());
			byte[] cppByte = Encoding.GetEncoding("ks_c_5601-1987").GetBytes(cppCode.ToCharArray());
			byte[] csByte = Encoding.GetEncoding("ks_c_5601-1987").GetBytes(csharpCode.ToCharArray());

			System.IO.File.WriteAllBytes(filename + ".h", hByte);
			System.IO.File.WriteAllBytes(filename + ".cpp", cppByte);
			System.IO.File.WriteAllBytes(filename + ".cs", csByte);

// 			System.IO.StreamWriter hOut = new System.IO.StreamWriter(filename + ".h");
// 			hOut.Write(hByte);
// 			hOut.Close();
//             System.IO.StreamWriter cppOut = new System.IO.StreamWriter(filename + ".cpp");
//             cppOut.Write(cppByte);
//             cppOut.Close();
//             System.IO.StreamWriter csOut = new System.IO.StreamWriter(filename + ".cs");
// 			csOut.Write(csByte);
// 			csOut.Close();
			return;

		}
	}
}