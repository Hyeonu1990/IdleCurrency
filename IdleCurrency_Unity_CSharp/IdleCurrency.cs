using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[System.Serializable]
public class IdleCurrency
{
    public BWFType type;
    public float num; // 표시될 숫자 Min : 1, Max 999.99
    public int killo_count; // 단위, 1000^killo_count

    #region 생성자
    public IdleCurrency(BWFType _type = BWFType.Idle_Gold)
    {
        type = _type;
        num = 0;
        killo_count = 0;
    }

    public IdleCurrency(double _input, BWFType _type = BWFType.Idle_Gold)
    {
        type = _type;
        if (_input == 0)
            return;
        while (num == 0)
        {
            if (_input / 1000 >= 1)
            {
                _input = _input / 1000;
                killo_count++;
            }
            else
            {
                num = (float)_input;
            }
        }
    }
    //public IdleCurrency(System.IComparable _input) : this((double)_input) { }

    public IdleCurrency(float _num, int _killo_count, BWFType _type = BWFType.Idle_Gold)
    {
        type = _type;
        if (_num == 0)
            return;
        //num = _num;
        killo_count = _killo_count;
        while (num == 0)
        {
            if (_num / 1000 >= 1)
            {
                _num = _num / 1000;
                killo_count++;
            }
            else
            {
                num = _num;
            }
        }
    }

    public IdleCurrency(float _num, string _kilo_str, BWFType _type = BWFType.Idle_Gold)
    {
        type = _type;
        if (_num == 0)
            return;
        //num = _num;
        if (string.IsNullOrEmpty(_kilo_str) || int.TryParse(_kilo_str, out int _output) == true /*|| _output == 0*/)
            killo_count = 0;
        else
        {
            for(int i = 0; i < _kilo_str.Length; i++)
            {
                var _index = (_kilo_str.Length - 1) - i;
                killo_count += (int)Mathf.Pow(26, _index) * (_kilo_str[i] - 'a' + 1);
            }
            //killo_count += 1;
        }
        while (num == 0)
        {
            if (_num / 1000 >= 1)
            {
                _num = _num / 1000;
                killo_count++;
            }
            else
            {
                num = _num;
            }
        }
    }
    #endregion

    #region 연산자 및 static
    public static readonly IdleCurrency Zero = new IdleCurrency();

    public static IdleCurrency operator -(IdleCurrency A) => new IdleCurrency(A.num * -1, A.killo_count, A.type);
    public static IdleCurrency operator +(IdleCurrency A, IdleCurrency B)
    {
        if(A.type != B.type)
        {
            throw new System.ArgumentException("다른 타입끼리 연산을 시도함");
        }
        var _temp = new IdleCurrency(A.type);
        _temp.killo_count = Mathf.Max(A.killo_count, B.killo_count);
        var diff = Mathf.Abs(A.killo_count - B.killo_count);
        if (diff == 1)
        {
            _temp.num = A.killo_count > B.killo_count ? A.num + Mathf.RoundToInt(B.num) / 10 / 100f : B.num + Mathf.RoundToInt(A.num) / 10 / 100f;
            if (_temp.num < 0)
            {
                _temp.num += 1000;
                _temp.killo_count -= 1;
            }
        }
        else if (diff == 0)
        {
            if (A.num + B.num >= 1000)
            {
                _temp.num = Mathf.RoundToInt(A.num + B.num) / 10 / 100f;
                _temp.killo_count++;
            }
            else
            {
                _temp.num = A.num + B.num;
            }
        }
        else
            _temp.num = A.killo_count > B.killo_count ? A.num + 0.01f * (B > 0 ? 1 : -1) : B.num + 0.01f * (A > 0 ? 1 : -1);

        var _num = _temp.num;
        while (_num != 0)
        {
            if (_num / 1000 >= 1)
            {
                _num = _num / 1000;
                _temp.killo_count++;
            }
            else
            {
                _temp.num = _num;
                _num = 0;
            }
        }
        return _temp;
    }    
    public static IdleCurrency operator -(IdleCurrency A, IdleCurrency B) => A + (-B);
    public static IdleCurrency operator +(IdleCurrency A, double _input) => A + new IdleCurrency(_input, A.type);
    public static IdleCurrency operator -(IdleCurrency A, double _input) => A + -(new IdleCurrency(_input, A.type));
    public static IdleCurrency operator *(IdleCurrency A, double _input)
    {
        var _temp = new IdleCurrency(A.num, A.killo_count, A.type);
        var _temp_num = A.num * _input;
        int _temp_kilo = 0;
        while (_temp_num != 0)
        {
            if (_temp_num / 1000 >= 1)
            {
                _temp_num /= 1000;
                _temp_kilo++;
            }
            else
            {
                _temp.num = (float)_temp_num;
                _temp_num = 0;
            }
        }
        _temp.killo_count += _temp_kilo;
        return _temp;
    }
    public static IdleCurrency operator /(IdleCurrency A, double _input)
    {
        if(_input == 0)
        {
            throw new System.ArgumentException("0으로 나눌수 읎다.");
        }
        var _temp = new IdleCurrency(A.num, A.killo_count, A.type);
        var _temp_num = A.num / _input;
        int _temp_kilo = 0;
        while (_temp_num != 0)
        {
            if (_temp_num < 0.01)
            {
                _temp_num *= 1000;
                _temp_kilo--;
            }
            else
            {
                _temp.num = (float)_temp_num;
                _temp_num = 0;
            }
        }
        _temp.killo_count += _temp_kilo;
        return _temp;
    }

    public static bool operator >(IdleCurrency A, IdleCurrency B)
    {
        if (A.type != B.type)
        {
            throw new System.ArgumentException("다른 타입끼리 비교를 시도함");
        }

        if (A.killo_count == B.killo_count)
        {
            return A.num > B.num;
        }
        else
        {
            return A.killo_count > B.killo_count;
        }
    }
    public static bool operator <(IdleCurrency A, IdleCurrency B) => B > A;
    public static bool operator >=(IdleCurrency A, IdleCurrency B)
    {
        if (A.type != B.type)
        {
            throw new System.ArgumentException("다른 타입끼리 연산을 시도함");
        }

        if (A.num == B.num && A.killo_count == B.killo_count)
            return true;
        else
            return A > B;
    }
    public static bool operator <=(IdleCurrency A, IdleCurrency B) => B >= A;
    public static bool operator >(IdleCurrency A, double _input) => A > new IdleCurrency(_input, A.type);
    public static bool operator <(IdleCurrency A, double _input) => new IdleCurrency(_input, A.type) > A;
    public static bool operator >=(IdleCurrency A, double _input) => A >= new IdleCurrency(_input, A.type);
    public static bool operator <=(IdleCurrency A, double _input) => new IdleCurrency(_input, A.type) >= A;
    public static bool operator ==(IdleCurrency A, IdleCurrency B)
    {
        if (A is null || B is null)
        {
            return (object)A == (object)B;
        }

        if (A.type != B.type)
        {
            throw new System.ArgumentException("다른 타입끼리 비교를 시도함");
        }

        return A.num == B.num && A.killo_count == B.killo_count;
    }
    public static bool operator !=(IdleCurrency A, IdleCurrency B) => !(A == B);
    #endregion

    public override bool Equals(object obj)
    {
        return base.Equals(obj);
    }

    public bool ValueEquals(IdleCurrency obj)
    {
        return this == obj;
    }

    public override int GetHashCode()
    {
        return base.GetHashCode();
    }

    public override string ToString()
    {
        string unit_string = "";
        if (killo_count > 0)
        {
            var _kilo_count = killo_count - 1;
            while (_kilo_count >= 0)
            {
                if (_kilo_count / 26 > 0)
                {
                    unit_string += (char)('a' + _kilo_count % 26);
                    _kilo_count /= 26;
                }
                else
                {
                    unit_string += (char)('a' + _kilo_count - ((killo_count - 1) / 26 > 0 ? 1 : 0));
                    _kilo_count = -1;
                }
            }
            var reverse_array = unit_string.ToCharArray();
            System.Array.Reverse(reverse_array);
            unit_string = new string(reverse_array);
        }
        return num.ToString(type == BWFType.Idle_Cash ? "F0" : "F2") + unit_string;
    }
}
