// Copyright (c) 2014, Intel Corporation
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

using System;
using System.Collections.Generic;
using System.Linq;
using Windows.Foundation.Collections;

namespace Samples.Common
{
    public class ObservableDictionary : IObservableMap<string, object> {
        private class ObservableDictionaryChangedEventArgs : IMapChangedEventArgs<string> {
            public ObservableDictionaryChangedEventArgs(CollectionChange change, string key) {
                this.CollectionChange = change;
                this.Key = key;
            }

            public CollectionChange CollectionChange { get; private set; }
            public string Key { get; private set; }
        }

        private Dictionary<string, object> _dictionary = new Dictionary<string, object>();
        public event MapChangedEventHandler<string, object> MapChanged;

        private void InvokeMapChanged(CollectionChange change, string key) {
            var eventHandler = MapChanged;
            if (eventHandler != null) {
                eventHandler(this, new ObservableDictionaryChangedEventArgs(change, key));
            }
        }

        public void Add(string key, object value) {
            this._dictionary.Add(key, value);
            this.InvokeMapChanged(CollectionChange.ItemInserted, key);
        }

        public void Add(KeyValuePair<string, object> item) {
            this.Add(item.Key, item.Value);
        }

        public bool Remove(string key) {
            if (this._dictionary.Remove(key)) {
                this.InvokeMapChanged(CollectionChange.ItemRemoved, key);
                return true;
            }

            return false;
        }

        public bool Remove(KeyValuePair<string, object> item) {
            object currentValue;

            if (this._dictionary.TryGetValue(item.Key, out currentValue) &&
                Object.Equals(item.Value, currentValue) && this._dictionary.Remove(item.Key)) {
                this.InvokeMapChanged(CollectionChange.ItemRemoved, item.Key);
                return true;
            }

            return false;
        }

        public object this[string key] {
            get { return this._dictionary[key]; }
            set {
                this._dictionary[key] = value;
                this.InvokeMapChanged(CollectionChange.ItemChanged, key);
            }
        }

        public void Clear() {
            var priorKeys = this._dictionary.Keys.ToArray();
            this._dictionary.Clear();
            foreach (var key in priorKeys) {
                this.InvokeMapChanged(CollectionChange.ItemRemoved, key);
            }
        }

        public ICollection<string> Keys {
            get { return this._dictionary.Keys; }
        }

        public bool ContainsKey(string key) {
            return this._dictionary.ContainsKey(key);
        }

        public bool TryGetValue(string key, out object value) {
            return this._dictionary.TryGetValue(key, out value);
        }

        public ICollection<object> Values {
            get { return this._dictionary.Values; }
        }

        public bool Contains(KeyValuePair<string, object> item) {
            return this._dictionary.Contains(item);
        }

        public int Count {
            get { return this._dictionary.Count; }
        }

        public bool IsReadOnly {
            get { return false; }
        }

        public IEnumerator<KeyValuePair<string, object>> GetEnumerator() {
            return this._dictionary.GetEnumerator();
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() {
            return this._dictionary.GetEnumerator();
        }

        public void CopyTo(KeyValuePair<string, object>[] array, int arrayIndex) {
            int arraySize = array.Length;
            foreach (var pair in this._dictionary) {
                if (arrayIndex >= arraySize) break;
                array[arrayIndex++] = pair;
            }
        }
    }
}
