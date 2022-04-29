using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading.Tasks;

namespace SFAI_ECG_WebApp
{
    /*
     * SFAI Interface class for implementing the C++ library consistently
     */
    public class SFAI_Interface
    {
        /*
         * External reference to CalcAll method in SFAI.DLL
         */
        [DllImport(@"SFAI.dll")]
        public static extern void CalcAll(char[] model_name, float[] all_chunks, int N_chunks, float[] ret_buf, IntPtr N_ret);

        public static float BPM(float n_beats, int n_chunks)
        {
            return 60.0f*n_beats / (6.0f * n_chunks);
        }

        /*
         * Basic test implementation loads and sets everything automatically.
         * Use for sanity checks, if needed. Utility otherwise is very low.
         */
        public static float TestCalc()
        {
            List<float> vals = new List<float>();
            foreach(string line in File.ReadLines("test_dat_good.txt"))
            {
                vals.Add(float.Parse(line));
            }
            Console.WriteLine("Vals has " + vals.Count.ToString());
            float[] v_buf = new float[32];
            IntPtr Nr = Marshal.AllocHGlobal(sizeof(int));
            char[] mname = {'f','d','e','e','p','_','m','o','d','e','l','.','j','s','o','n','\0' };// "fdeep_model.json";

            CalcAll(mname, vals.ToArray(), 1, v_buf, Nr);
            return v_buf[0];
        }

        /*
         * Perform the calculation on a specific file
         * @param fname The path/filename to load from. Must be composed of [value]\n entries for each ECG sample
         */
        public static float CalcFile(string fname)
        {
            //Create a new list of floats and populate it from the file
            List<float> vals = new List<float>();
            foreach (string line in File.ReadLines(fname))
            {
                vals.Add(float.Parse(line));
            }
            Console.WriteLine("Vals has " + vals.Count.ToString());

            //If the file doesn't contain a multiple of 2160 samples, we must make it match
            if(vals.Count % 2160 > 0)
            {
                for(int i = 0; i < 2160 - vals.Count % 2160; i++)
                {
                    vals.Add(0);
                }
            }
            //Output buffer to feed in to CalcAll
            float[] v_buf = new float[32];
            //int* to be set within SFAI. Will indicate the number of populated buffer entries
            IntPtr Nr = Marshal.AllocHGlobal(sizeof(int));
            //Kludge to make C# char array work in C++
            //Will probably deprecate and just have a single model file hard-coded
            char[] mname = { 'f', 'd', 'e', 'e', 'p', '_', 'm', 'o', 'd', 'e', 'l', '.', 'j', 's', 'o', 'n', '\0' };// "fdeep_model.json";
            //Number of chunks - eg. the number of 2160-size float arrays
            int Nc = vals.Count / 2160;
            Console.WriteLine("Found " + Nc.ToString() + " chunks");
            //Execute CalcAll on the data
            CalcAll(mname, vals.ToArray(), Nc, v_buf, Nr);
            //Accumulate the number of beats detected in each chunk
            float accum = 0;
            for (int i = 0; i < Marshal.ReadInt32(Nr); i++)
            {
                accum += v_buf[i];
            }
            //Return the calculated BPM of the ECG
            return BPM(accum, Marshal.ReadInt32(Nr));
            //return accum;
        }

        public static T ParseBytes<T>(byte[] bytes)
        {
            var handle = GCHandle.Alloc(bytes, GCHandleType.Pinned);
            var structure = Marshal.PtrToStructure(handle.AddrOfPinnedObject(), typeof(T));
            handle.Free();

            return (T)structure;
        }

        /*
         * Perform the calculation on a byte array
         * @param bytes The array of bytes to generate a float array to use as input
         */
        public static float CalcFromBytes(byte[] data)
        {
            List<float> vals = new List<float>();
            if(data.Length > 0)
            {
                MemoryStream ms = new MemoryStream(data);
                int sz = Marshal.SizeOf(typeof(float));
                byte[] smpbuf = new byte[sz];
                while(ms.Read(smpbuf,0,sz) > 0)
                {
                    var res = ParseBytes<float>(smpbuf);
                    vals.Add(res);
                }
                Console.WriteLine("Vals has " + vals.Count.ToString());
                if (vals.Count % 2160 > 0)
                {
                    for (int i = 0; i < 2160 - vals.Count % 2160; i++)
                    {
                        vals.Add(0);
                    }
                }
                float[] v_buf = new float[32];
                IntPtr Nr = Marshal.AllocHGlobal(sizeof(int));
                char[] mname = { 'f', 'd', 'e', 'e', 'p', '_', 'm', 'o', 'd', 'e', 'l', '.', 'j', 's', 'o', 'n', '\0' };// "fdeep_model.json";
                //get number of 2160 sample chunks contained in data
                int Nc = (int)Math.Ceiling((decimal)vals.Count/2160);
                CalcAll(mname, vals.ToArray(), Nc, v_buf, Nr);
                float accum = 0;
                for(int i = 0; i < Marshal.ReadInt32(Nr); i++)
                {
                    accum += v_buf[i];
                }
                return accum;
            }
            return -1;
        }
    }
}
