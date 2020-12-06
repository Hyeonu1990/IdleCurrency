//
//  IdleCurrency.hpp
//  testproject
//
//  Created by hyeonu choi on 2020/12/05.
//

#ifndef IdleCurrency_hpp
#define IdleCurrency_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>

namespace Idle
{
    enum BWFType
    {
        Idle_Gold,
        Idle_Heart,
        Idle_Cash
    };

    class IdleCurrency
    {
    //value
    public:
        BWFType type;
        float num = 0; // 표시될 숫자 Min : 1, Max 999.99
        int killo_count = 0; // 단위, 1000^killo_count
        
    //생성자
    public:
        IdleCurrency(BWFType _type = BWFType::Idle_Gold)
        {
            type = _type;
            num = 0;
            killo_count = 0;
        }

        IdleCurrency(double _input, BWFType _type = BWFType::Idle_Gold)
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

        IdleCurrency(float _num, int _killo_count, BWFType _type = BWFType::Idle_Gold)
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

        IdleCurrency(float _num, std::string _kilo_str, BWFType _type = BWFType::Idle_Gold)
        {
            type = _type;
            if (_num == 0)
                return;
            bool isNumber = false;
            int str_int_value = 0;
            try
            {
                str_int_value = std::stoi(_kilo_str);
                isNumber = true;
            }
            catch (const std::exception& expn)
            {
                //std::cout << expn.what() << " Not Integer\n";
                isNumber = false;
            }
            catch (...)
            {
                std::cout << ": Unknown error\n";
            }
            if (_kilo_str.empty() || isNumber)
            {
                killo_count = isNumber ? str_int_value : 0;
            }
            else
            {
                for(int i = 0; i < _kilo_str.length(); i++)
                {
                    int _index = ((int)_kilo_str.length() - 1) - i;
                    killo_count += (int)pow(26, _index) * (_kilo_str[i] - 'a' + 1);
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

    //연산자 및 static
    public:
        IdleCurrency& operator =(const IdleCurrency &B)
        {
            try
            {
                if (this->type != B.type)
                {
                    throw 0;
                }
            }
            catch (int ex)
            {
                std::cout << "다른 타입끼리 비교를 시도함" << std::endl;
            }
            if(*this == B)
                return *this;
            this->num = B.num;
            this->killo_count = B.killo_count;
            return *this;
        }
        IdleCurrency& operator -()
        {
            this->num *= -1;
            return *this;
        }
        const IdleCurrency operator +(const IdleCurrency &B)
        {
            try
            {
                if (this->type != B.type)
                {
                    throw 0;
                }
            }
            catch (int ex)
            {
                std::cout << "다른 타입끼리 비교를 시도함" << std::endl;
            }
            IdleCurrency _temp(this->type);
            _temp.killo_count = this->killo_count >= B.killo_count ? this->killo_count : B.killo_count;
            int diff = abs(this->killo_count - B.killo_count);
            if (diff == 1)
            {
                _temp.num = this->killo_count > B.killo_count ? this->num + (int)round(B.num) / 10 / 100.0f : B.num + (int)round(this->num) / 10 / 100.0f;
                if (_temp.num < 0)
                {
                    _temp.num += 1000;
                    _temp.killo_count -= 1;
                }
            }
            else if (diff == 0)
            {
                if (this->num + B.num >= 1000)
                {
                    _temp.num = (int)round(this->num + B.num) / 10 / 100.0f;
                    _temp.killo_count++;
                }
                else
                {
                    _temp.num = this->num + B.num;
                }
            }
            else if (diff > 1)
                _temp.num = this->killo_count > B.killo_count ? this->num + 0.01f * (B.num >= 0 ? 1 : -1) : B.num + 0.01f * (this->num >= 0 ? 1 : -1);
            
            float _num = _temp.num;
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
        IdleCurrency& operator +=(const IdleCurrency &B)
        {
            *this = *this + B;
            return *this;
        }
        const IdleCurrency operator -(const IdleCurrency &B)
        {
            IdleCurrency _input = B;
            return *this + -_input;
        }
        IdleCurrency& operator -=(const IdleCurrency &B)
        {
            *this = *this - B;
            return *this;
        }
        const IdleCurrency operator +(double _input)
        {
            return *this + IdleCurrency(_input, this->type);
        }
        IdleCurrency& operator +=(double _input)
        {
            *this = *this + _input;
            return *this;
        }
        const IdleCurrency operator -(double _input)
        {
            return *this + -(IdleCurrency(_input, this->type));
        }
        IdleCurrency& operator -=(double _input)
        {
            *this = *this - _input;
            return *this;
        }
        const IdleCurrency operator *(double _input)
        {
            IdleCurrency _temp(this->num, this->killo_count, this->type);
            float _temp_num = this->num * _input;
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
        IdleCurrency& operator *=(double _input)
        {
            *this = *this * _input;
            return *this;
        }
        const IdleCurrency operator /(double _input)
        {
            try
            {
                if(_input == 0)
                {
                    throw 0;
                }
            }
            catch (int ex)
            {
                std::cout << "0으로 나눌수 읎다." << std::endl;
            }
            
            IdleCurrency _temp(this->num, this->killo_count, this->type);
            float _temp_num = this->num / _input;
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
        IdleCurrency& operator /=(double _input)
        {
            *this = *this / _input;
            return *this;
        }
        bool operator >(IdleCurrency B)
        {
            try
            {
                if (this->type != B.type)
                {
                    throw 0;
                }
            }
            catch (int ex)
            {
                std::cout << "다른 타입끼리 비교를 시도함" << std::endl;
            }

            if (this->killo_count == B.killo_count)
            {
                return this->num > B.num;
            }
            else
            {
                return this->killo_count > B.killo_count;
            }
        }
        bool operator <(const IdleCurrency &B)
        {
            IdleCurrency _input = B;
            return _input > *this;
        }
        bool operator >=(const IdleCurrency &B)
        {
            try
            {
                if (this->type != B.type)
                {
                    throw 0;
                }
            }
            catch (int ex)
            {
                std::cout << "다른 타입끼리 비교를 시도함" << std::endl;
            }

            if (this->num == B.num && this->killo_count == B.killo_count)
                return true;
            else
                return *this > B;
        }
        bool operator <=(const IdleCurrency &B)
        {
            IdleCurrency _input = B;
            return _input >= *this;
        }
        bool operator >(double _input)
        {
            return *this > IdleCurrency(_input, this->type);
        }
        bool operator <(double _input)
        {
            return IdleCurrency(_input, this->type) > *this;
        }
        bool operator >=(double _input)
        {
            return *this >= IdleCurrency(_input, this->type);
        }
        bool operator <=(double _input)
        {
            return IdleCurrency(_input, this->type) >= *this;
        }
        bool operator ==(const IdleCurrency &B)
        {
            try
            {
                if (!this || &B == NULL)
                {
                    throw 0;
                }
            }
            catch (int ex)
            {
                std::cout << "nullptr" << std::endl;
            }

            try
            {
                if (this->type != B.type)
                {
                    throw 0;
                }
            }
            catch (int ex)
            {
                std::cout << "다른 타입끼리 비교를 시도함" << std::endl;
            }
            return this->num == B.num && this->killo_count == B.killo_count;
        }
        bool operator !=(IdleCurrency B)
        {
            return !(*this == B);
        }

        bool ValueEquals(IdleCurrency obj)
        {
            return *this == obj;
        }
        
        std::string ToString()
        {
            std::string unit_string = "";
            if (killo_count > 0)
            {
                int _kilo_count = killo_count - 1;
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
                std::reverse(unit_string.begin(), unit_string.end());
            }
            return (type == BWFType::Idle_Cash ? std::to_string((int)num) : std::to_string(num)) + unit_string;
        }
    };
}

#endif /* IdleCurrency_hpp */
